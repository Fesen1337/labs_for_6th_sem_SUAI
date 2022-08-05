#pragma once

#include <iostream>
#include <vector>

template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &v) {
  stream << "vector [" << v.size() << "] {\n";
  for (const T &val : v) {
    stream << "  " << val << "\n";
  }
  return stream << "}";
}
