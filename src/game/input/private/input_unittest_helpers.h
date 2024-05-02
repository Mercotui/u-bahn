#pragma once

#include <string>

namespace InputUnittestHelpers {
template <class ListType>
constexpr typename std::iterator_traits<typename ListType::iterator>::difference_type CountInputType(
    const ListType& list, Input::Type expected_type) {
  return std::count_if(std::cbegin(list), std::cend(list),
                       [expected_type](const auto& input) { return input->type == expected_type; });
}

template <class ListType>
constexpr typename std::iterator_traits<typename ListType::iterator>::difference_type CountInputName(
    const ListType& list, const std::string& expected_name) {
  return std::count_if(std::cbegin(list), std::cend(list),
                       [expected_name](const auto& input) { return input->name == expected_name; });
}
}  // namespace InputUnittestHelpers
