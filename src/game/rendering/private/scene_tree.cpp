// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#include "game/rendering/scene_tree.h"

#include <absl/log/check.h>

#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {
std::array<float, 16> PlaceholderTransform{};
using ModelSpanMap = std::unordered_map<ModelDataId, std::vector<SceneNode::TransformSpan>>;

/**
 * Do a breadth first traversal of the node and all of its children.
 * @param start the node at the root of the traversal, it and all of its children will be visited passed to predicate
 * @param predicate the predicate called with each node, this predicate may alter the children of the passed node
 */
void BreadthFirstForEach(SceneNode* start, const std::function<void(SceneNode*)>& predicate) {
  std::queue<SceneNode*> queue;
  queue.push(start);
  while (!queue.empty()) {
    SceneNode* node = queue.front();
    queue.pop();

    predicate(node);

    for (auto& child : node->children) {
      queue.push(child.get());
    }
  }
}

/// Create new transform spans for the whole tree if the vector was reallocated
void UpdateReallocatedTransform(SceneNode* node, const float* old_begin, const float* old_end, float* new_begin) {
  const float* span_begin = node->transform.data();
  const float* span_end = span_begin + SceneNode::TransformSpan::extent;

  // Check that the span was inside the vector
  CHECK_LE(old_begin, span_begin);
  CHECK_GE(old_end, span_end);

  const auto offset = span_begin - old_begin;
  node->transform = SceneNode::TransformSpan(new_begin + offset, SceneNode::TransformSpan::extent);
}

/// Create new a transform span for this node if the corresponding vector had sections erased
void UpdateShiftedTransform(SceneNode* node, SceneTree::ModelTransformsMap* transforms_map,
                            const ModelSpanMap& erased_spans_map) {
  // Find if this model was affected
  const auto erased_spans_it = erased_spans_map.find(node->model);
  if (erased_spans_it == erased_spans_map.end()) {
    return;
  }
  const auto& erased_spans = erased_spans_it->second;

  // Find the transforms for this model
  const auto transforms_it = transforms_map->find(node->model);
  CHECK(transforms_it != transforms_map->end()) << "Trying to shift transform in nonexistent vector";
  auto& transforms = transforms_it->second;

  // Deduct any transforms if needed
  const float* span_begin = node->transform.data();
  ptrdiff_t span_offset = span_begin - transforms.data();
  for (const auto& erased_span : erased_spans) {
    // If this erased span came before this node's span, then we deduct one span's width
    if (erased_span.data() < span_begin) {
      span_offset -= SceneNode::TransformSpan::extent;
    }
  }
  node->transform = SceneNode::TransformSpan(transforms.data() + span_offset, SceneNode::TransformSpan::extent);
}

SceneNode::TransformSpan CreateSpan(SceneTree::ModelTransformsMap* transforms_map, SceneNode* tree,
                                    const ModelDataId& model) {
  if (!model) {
    return PlaceholderTransform;
  }

  auto& transforms = (*transforms_map)[model];

  const auto transforms_size_before = transforms.size();
  const auto transforms_size_after = transforms_size_before + SceneNode::TransformSpan::extent;

  const auto had_data_before = !transforms.empty();
  const auto base_before = transforms.data();
  const auto end_before = base_before + transforms.size();
  transforms.resize(transforms_size_after, 0.0f);
  const auto base_after = transforms.data();

  if (had_data_before && base_before != base_after) {
    BreadthFirstForEach(tree, [&](auto* node) {
      if (model == node->model) {
        UpdateReallocatedTransform(node, base_before, end_before, base_after);
      }
    });
  }

  return SceneNode::TransformSpan(base_after + transforms_size_before, SceneNode::TransformSpan::extent);
}

void EraseSpan(SceneTree::ModelTransformsMap* transforms_map, const ModelDataId model,
               const SceneNode::TransformSpan& span) {
  const auto transforms_it = transforms_map->find(model);
  CHECK(transforms_it != transforms_map->end()) << "Trying to erase transform from nonexistent vector";

  // Erase the span
  auto& vec = transforms_it->second;
  const auto first = vec.begin() + (span.data() - vec.data());
  const auto last = first + span.size();
  vec.erase(first, last);

  // Erase the entry if there are no transforms left for this model
  if (transforms_it->second.empty()) {
    transforms_map->erase(transforms_it);
  }
}
}  // namespace

void SceneNode::SetTransform(glm::mat4 value) const {
  static_assert(sizeof(TransformSpan::value_type) * TransformSpan::extent == sizeof(glm::mat4),
                "TransformSpan and glm::mat4 are different sizes");
  std::memcpy(transform.data(), glm::value_ptr(value), transform.size_bytes());
}

SceneTree::SceneTree() : root_{.model = 0, .transform = PlaceholderTransform} {}

SceneNode* SceneTree::GetRoot() { return &root_; }

const SceneTree::ModelTransformsMap& SceneTree::GetModelTransforms() const { return model_transform_buffers_; }

SceneNode* SceneTree::Insert(SceneNode* parent, const ModelDataId& model) {
  auto transform = CreateSpan(&model_transform_buffers_, &root_, model);
  auto new_node = std::make_unique<SceneNode>(model, transform);
  const auto new_node_raw = new_node.get();
  parent->children.push_back(std::move(new_node));
  return new_node_raw;
}

std::unordered_set<ModelDataId> SceneTree::Remove(SceneNode* node_to_erase) {
  // Find all child nodes
  ModelSpanMap erased_spans;
  BreadthFirstForEach(node_to_erase, [&](auto child) {
    if (child->model) {
      erased_spans[child->model].push_back(child->transform);
    }
  });

  // Sort the spans from back to front and then erase them
  std::ranges::for_each(erased_spans, [buffers = &model_transform_buffers_](auto& entry) {
    auto& [model, spans] = entry;
    std::ranges::sort(spans, [](auto& a, auto& b) { return a.data() > b.data(); });
    std::ranges::for_each(spans, [buffers, model](auto& span) { EraseSpan(buffers, model, span); });
  });

  // Check each node in the tree if it should be deleted or updated
  std::unordered_set<ModelDataId> models_still_in_use;
  BreadthFirstForEach(&root_, [&](auto node) {
    std::erase_if(node->children, [node_to_erase](auto& child) { return node_to_erase == child.get(); });
    if (node->model) {
      models_still_in_use.insert(node->model);
      UpdateShiftedTransform(node, &model_transform_buffers_, erased_spans);
    }
  });

  return models_still_in_use;
}
