#pragma once

template <class... Ts>
struct OverloadedVisit : Ts... {
  using Ts::operator()...;
};
