#include <gtest/gtest.h>

#include <algorithm>

#include "game/input/input_manager_interface.h"
#include "platform/platform.h"

namespace {
template <class ListType>
constexpr typename std::iterator_traits<typename ListType::iterator>::difference_type CountInputType(
    const ListType& list, Input::Type expected_type) {
  return std::count_if(std::cbegin(list), std::cend(list),
                       [expected_type](const Input::Info& info) { return info.type == expected_type; });
}
}  // namespace

TEST(InputManagerFactory, InputListAndroid) {
  const auto manager = InputManagerFactory::Create(Platform::Type::kAndroid);
  const auto list = manager->ListInputs();

  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(CountInputType(list, Input::Type::kTouch), 1);
}

TEST(InputManagerFactory, InputListWeb) {
  const auto manager = InputManagerFactory::Create(Platform::Type::kWeb);
  const auto list = manager->ListInputs();

  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(CountInputType(list, Input::Type::kKeyboard), 1);
  EXPECT_EQ(CountInputType(list, Input::Type::kMouse), 1);
  EXPECT_EQ(CountInputType(list, Input::Type::kTouch), 1);
}

TEST(InputManagerFactory, InputListDesktop) {
  const auto manager = InputManagerFactory::Create(Platform::Type::kDesktop);
  const auto list = manager->ListInputs();

  // We can not check for the length of this list, as this depends on the hardware connected at runtime.
  // We only check the minimum list of expected devices.
  EXPECT_EQ(CountInputType(list, Input::Type::kKeyboard), 1);
  EXPECT_EQ(CountInputType(list, Input::Type::kMouse), 1);
}
