// SPDX-FileCopyrightText: 2025 Menno van der Graaf <mennovandergraaf@hotmail.com>
// SPDX-License-Identifier: MIT
#include "game/rendering/scene_tree.h"

#include <gtest/gtest.h>

#include <glm/ext/matrix_transform.hpp>
#include <vector>

TEST(SceneTreeTest, Single) {
  constexpr ModelDataId kModelId{42};
  SceneTree tree;
  const auto root = tree.GetRoot();
  EXPECT_EQ(root->children.size(), 0);

  const auto node = tree.Insert(root, kModelId);
  EXPECT_EQ(node->model, kModelId);
  EXPECT_EQ(root->children.size(), 1);
  EXPECT_EQ(root->children.front().get(), node);

  node->SetTransform(glm::identity<glm::mat4>());

  const auto& transforms_map = tree.GetModelTransforms();
  EXPECT_EQ(transforms_map.size(), 1);
  const auto& model_transforms = transforms_map.at(kModelId);
  EXPECT_EQ(model_transforms.size(), 16);

  // clang-format off
  EXPECT_EQ(model_transforms, std::vector({
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  }));
  // clang-format on

  tree.Remove(node);
  EXPECT_EQ(root->children.size(), 0);
  EXPECT_EQ(transforms_map.size(), 0);
}

/**
 * This test tries to run the reallocation code by inserting many nodes.
 * The actual limits before reallocation of the transforms happens probably varies per system.
 * To check that the reallocation code is used, set a breakpoint on scene_tree.cpp:UpdateReallocatedTransform.
 */
TEST(SceneTreeTest, Many) {
  SceneTree tree;
  const auto root = tree.GetRoot();
  const auto parent_to_all = tree.Insert(root, {0});

  // Test configuration
  constexpr ModelDataId kModelId{666};
  constexpr auto kNumberOfNodes{1000};
  constexpr auto kStoreEveryNthNode{100};
  std::vector<SceneNode*> every_nth_node;

  // Add the nodes
  for (int node_count = 0; node_count < kNumberOfNodes; ++node_count) {
    const auto node = tree.Insert(parent_to_all, kModelId);
    if (node_count % kStoreEveryNthNode == 0) {
      every_nth_node.push_back(node);
    }
  }

  // Check the nodes all ended up in the tree
  EXPECT_EQ(root->children.size(), 1);
  EXPECT_EQ(parent_to_all->children.size(), kNumberOfNodes);

  // Check the transforms for this model
  const auto& transforms_map = tree.GetModelTransforms();
  EXPECT_EQ(transforms_map.size(), 1);
  const auto& model_transforms = transforms_map.at(kModelId);
  EXPECT_EQ(model_transforms.size(), kNumberOfNodes * 16);

  // Check that a value assigned to the Nth nodes shows up in the transforms at the right place
  for (size_t i = 0; i < every_nth_node.size(); ++i) {
    const auto test_value = static_cast<float>(i);
    every_nth_node.at(i)->transform.front() = test_value;
    const auto global_index = i * kStoreEveryNthNode * 16;
    EXPECT_EQ(model_transforms.at(global_index), test_value);
  }

  // Cleanup
  tree.Remove(parent_to_all);
  EXPECT_EQ(root->children.size(), 0);
  EXPECT_EQ(transforms_map.size(), 0);
}

TEST(SceneTreeTest, ErasePrevious) {
  constexpr ModelDataId kModelId{20251230};
  SceneTree tree;
  const auto root = tree.GetRoot();
  const auto node_1 = tree.Insert(root, kModelId);
  const auto node_2 = tree.Insert(root, kModelId);
  const auto node_3 = tree.Insert(root, kModelId);
  const auto node_4 = tree.Insert(root, kModelId);

  EXPECT_EQ(root->children.size(), 4);
  EXPECT_EQ(root->children.at(0).get(), node_1);
  EXPECT_EQ(root->children.at(1).get(), node_2);
  EXPECT_EQ(root->children.at(2).get(), node_3);
  EXPECT_EQ(root->children.at(3).get(), node_4);

  const auto& transforms_map = tree.GetModelTransforms();
  EXPECT_EQ(transforms_map.size(), 1);
  const auto& model_transforms = transforms_map.at(kModelId);
  EXPECT_EQ(model_transforms.size(), 16 * 4);

  // Assign values to all nodes
  node_1->transform.front() = 11.0f;
  node_2->transform.front() = 12.0f;
  node_3->transform.front() = 13.0f;
  node_4->transform.front() = 14.0f;
  EXPECT_EQ(model_transforms.at(0), 11.0f);
  EXPECT_EQ(model_transforms.at(16), 12.0f);
  EXPECT_EQ(model_transforms.at(16 * 2), 13.0f);
  EXPECT_EQ(model_transforms.at(16 * 3), 14.0f);

  // Remove the second node, the remaining data should shift over, and assigning new data should still work
  tree.Remove(node_2);
  EXPECT_EQ(model_transforms.at(0), 11.0f);
  EXPECT_EQ(model_transforms.at(16), 13.0f);
  EXPECT_EQ(model_transforms.at(16 * 2), 14.0f);
  node_1->transform.front() = 21.0f;
  node_3->transform.front() = 23.0f;
  node_4->transform.front() = 24.0f;
  EXPECT_EQ(model_transforms.at(0), 21.0f);
  EXPECT_EQ(model_transforms.at(16), 23.0f);
  EXPECT_EQ(model_transforms.at(16 * 2), 24.0f);

  // Remove the first node and verify the remaining two
  tree.Remove(node_1);
  EXPECT_EQ(model_transforms.at(0), 23.0f);
  EXPECT_EQ(model_transforms.at(16), 24.0f);
  node_3->transform.front() = 33.0f;
  node_4->transform.front() = 34.0f;
  EXPECT_EQ(model_transforms.at(0), 33.0f);
  EXPECT_EQ(model_transforms.at(16), 34.0f);

  // Remove the third node and verify the forth
  tree.Remove(node_3);
  EXPECT_EQ(model_transforms.at(0), 34.0f);
  node_4->transform.front() = 44.0f;
  EXPECT_EQ(model_transforms.at(0), 44.0f);

  // remove the 4th node, none should remain
  tree.Remove(node_4);
  EXPECT_EQ(root->children.size(), 0);
  EXPECT_EQ(transforms_map.size(), 0);
}

TEST(SceneTreeTest, EraseRecursive) {
  SceneTree tree;
  const auto root = tree.GetRoot();
  const auto& transforms_map = tree.GetModelTransforms();

  constexpr ModelDataId kModelId1{101};
  constexpr ModelDataId kModelId2{102};
  constexpr ModelDataId kModelId3{103};
  constexpr ModelDataId kModelId4{104};

  const auto node_a = tree.Insert(root, kModelId1);
  const auto node_b = tree.Insert(root, kModelId1);

  const auto node_c = tree.Insert(node_a, kModelId2);
  const auto node_d = tree.Insert(node_a, kModelId2);
  const auto node_e = tree.Insert(node_d, kModelId3);

  const auto node_f = tree.Insert(node_b, kModelId2);
  const auto node_g = tree.Insert(node_b, kModelId2);
  const auto node_h = tree.Insert(node_g, kModelId4);

  EXPECT_EQ(root->children.size(), 2);

  node_a->transform.front() = 1.0f;
  node_b->transform.front() = 2.0f;
  node_c->transform.front() = 3.0f;
  node_d->transform.front() = 4.0f;
  node_e->transform.front() = 5.0f;
  node_f->transform.front() = 6.0f;
  node_g->transform.front() = 7.0f;
  node_h->transform.front() = 8.0f;

  const auto& transforms_1 = transforms_map.at(kModelId1);
  const auto& transforms_2 = transforms_map.at(kModelId2);
  const auto& transforms_3 = transforms_map.at(kModelId3);
  const auto& transforms_4 = transforms_map.at(kModelId4);
  EXPECT_EQ(transforms_1.size(), 16 * 2);
  EXPECT_EQ(transforms_2.size(), 16 * 4);
  EXPECT_EQ(transforms_3.size(), 16);
  EXPECT_EQ(transforms_4.size(), 16);

  EXPECT_EQ(transforms_1.at(0), 1.0f);
  EXPECT_EQ(transforms_1.at(16), 2.0f);

  EXPECT_EQ(transforms_2.at(0), 3.0f);
  EXPECT_EQ(transforms_2.at(16), 4.0f);
  EXPECT_EQ(transforms_2.at(16 * 2), 6.0f);
  EXPECT_EQ(transforms_2.at(16 * 3), 7.0f);

  EXPECT_EQ(transforms_3.at(0), 5.0f);
  EXPECT_EQ(transforms_4.at(0), 8.0f);

  tree.Remove(node_a);
  EXPECT_EQ(transforms_1.size(), 16 * 1);
  EXPECT_EQ(transforms_2.size(), 16 * 2);
  EXPECT_FALSE(transforms_map.contains(kModelId3));
  EXPECT_EQ(transforms_4.size(), 16);

  EXPECT_EQ(transforms_1.at(0), 2.0f);
  EXPECT_EQ(transforms_2.at(0), 6.0f);
  EXPECT_EQ(transforms_2.at(16), 7.0f);
  EXPECT_EQ(transforms_4.at(0), 8.0f);

  node_b->transform.front() = 11.0f;
  node_f->transform.front() = 12.0f;
  node_g->transform.front() = 13.0f;
  node_h->transform.front() = 14.0f;
  EXPECT_EQ(transforms_1.at(0), 11.0f);
  EXPECT_EQ(transforms_2.at(0), 12.0f);
  EXPECT_EQ(transforms_2.at(16), 13.0f);
  EXPECT_EQ(transforms_4.at(0), 14.0f);
}
