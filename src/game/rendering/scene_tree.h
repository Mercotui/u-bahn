// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#pragma once
#include <memory>
#include <span>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "game/rendering/model.h"

struct SceneNode {
  using TransformSpan = std::span<float, 16>;

  /// Identifier of the corresponding model
  ModelDataId model;
  /// Global 4x4 transform matrix of this model
  TransformSpan transform;
  /// Children nodes
  std::vector<std::unique_ptr<SceneNode>> children;

  /**
   * Helper function to assign a mat4 to the transform
   * @param value The new value for the transform
   */
  void SetTransform(glm::mat4 value) const;
};

/**
 * The scene tree keeps track of each model that needs to be rendered, and it's corresponding transform.
 * The goal of the scene tree is to glue together the physics with the rendering,
 * without hindering the performance of either subsystem.
 *
 * This is achieved by keeping a continuous block of transforms for each model,
 * and each node in the tree has access to a span across one transform.
 */
class SceneTree {
 public:
  using ModelTransformsMap = std::unordered_map<ModelDataId, std::vector<float>>;

  /**
   * Create an empty scene
   */
  SceneTree();

  /**
   * Access the scene root
   * @return The scene root
   */
  SceneNode* GetRoot();

  /**
   * Get the transforms for each model in the scene
   * @returns a map of continuous transform data for each instance of each model
   */
  const ModelTransformsMap& GetModelTransforms() const;

  /**
   * Add a node to the parent.
   * @param parent the parent to add the node to
   * @param model the model for this node
   * @returns the newly created node
   */
  SceneNode* Insert(SceneNode* parent, const ModelDataId& model);

  /**
   * Remove this node and all child-nodes.
   * @param node_to_erase the node to remove
   * @returns all the models still in the scene, can be used to prune the model cache
   */
  std::unordered_set<ModelDataId> Remove(SceneNode* node_to_erase);

 private:
  SceneNode root_;
  ModelTransformsMap model_transform_buffers_;
};
