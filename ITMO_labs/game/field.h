#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

class field_t {
public:
  static constexpr char EMPTY_CELL = '.';
  static constexpr char SHIP_CELL = '#';
  static constexpr char MISS_CELL = '0';
  static constexpr char HIT_CELL = '*';

  static constexpr size_t FIELD_SIZE = 10;

  explicit field_t(uint32_t seed);
  explicit field_t(std::vector<std::string> field);
  field_t(std::initializer_list<std::string> field) : field_t(std::vector<std::string>(field)) {}

  std::string &operator[](size_t pos) {
    return field[pos];
  }
  const std::string &operator[](size_t pos) const {
    return field[pos];
  }

  friend std::ostream &operator<<(std::ostream &stream, const field_t &field);

  static bool is_cell_valid(int x, int y) {
    return x >= 0 && y >= 0 && x < FIELD_SIZE && y < FIELD_SIZE;
  }

  friend bool operator==(const field_t &a, const field_t &b) {
    return a.field == b.field;
  }
  friend bool operator!=(const field_t &a, const field_t &b) {
    return !(a == b);
  }

  static constexpr std::array<std::pair<int, int>, 4> DIRECTIONS = {std::make_pair(0, 1), std::make_pair(0, -1),
                                                                    std::make_pair(1, 0), std::make_pair(-1, 0)};

private:
  std::vector<std::string> field;
};
