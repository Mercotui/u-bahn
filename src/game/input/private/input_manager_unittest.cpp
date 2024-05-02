#include <gtest/gtest.h>

#include "game/input/input_manager_interface.h"
#include "game/input/private/input_unittest_helpers.h"
#include "platform/platform.h"

using Type = Input::Type;
using InputUnittestHelpers::CountInputType;

TEST(InputManagerFactoryTest, InputListAndroid) {
  const auto manager = InputManagerFactory::Create(Platform::Type::kAndroid);
  const auto list = manager->Poll();

  EXPECT_EQ(list.size(), 1);
  EXPECT_EQ(CountInputType(list, Type::kTouch), 1);
}

TEST(InputManagerFactoryTest, InputListWeb) {
  const auto manager = InputManagerFactory::Create(Platform::Type::kWeb);
  const auto list = manager->Poll();

  EXPECT_EQ(list.size(), 3);
  EXPECT_EQ(CountInputType(list, Type::kKeyboard), 1);
  EXPECT_EQ(CountInputType(list, Type::kMouse), 1);
  EXPECT_EQ(CountInputType(list, Type::kTouch), 1);
}

TEST(InputManagerFactoryTest, InputListDesktop) {
  const auto manager = InputManagerFactory::Create(Platform::Type::kDesktop);
  const auto list = manager->Poll();

  // We can not check for the length of this list, as this depends on the hardware connected at runtime.
  // We only check the minimum list of expected devices.
  EXPECT_EQ(CountInputType(list, Type::kKeyboard), 1);
  EXPECT_EQ(CountInputType(list, Type::kMouse), 1);
}
