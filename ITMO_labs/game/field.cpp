#include "game/field.h"

#include <algorithm>
#include <cassert>
#include <random>

field_t::field_t(uint32_t seed) : field(FIELD_SIZE, std::string(FIELD_SIZE, EMPTY_CELL)) {
  std::seed_seq seq = {seed};
  std::mt19937_64 gen(seq);

  std::vector<size_t> ships_len = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
  std::shuffle(ships_len.begin(), ships_len.end(), gen);

  const auto check_cell = [&, this](int x, int y) {
    if (!is_cell_valid(x, y) || field[x][y] != EMPTY_CELL) {
      return false;
    }
    return std::all_of(DIRECTIONS.begin(), DIRECTIONS.end(), [&, this](const std::pair<int, int> &dir) {
      int dx = x + dir.first;
      int dy = y + dir.second;
      return !(is_cell_valid(dx, dy) && field[dx][dy] == SHIP_CELL);
    });
  };

  while (true) {
    field = std::vector<std::string>(FIELD_SIZE, std::string(FIELD_SIZE, EMPTY_CELL));
    bool flag = true;
    for (size_t ship_len : ships_len) {
      std::vector<std::tuple<int, int, size_t>> available_positions;
      for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
          for (size_t k = 0; k < DIRECTIONS.size(); ++k) {
            bool able = true;
            for (int q = 0; q < ship_len; ++q) {
              able &= check_cell(i + q * DIRECTIONS[k].first, j + q * DIRECTIONS[k].second);
            }
            if (able) {
              available_positions.emplace_back(i, j, k);
            }
          }
        }
      }

      if (available_positions.empty()) {
        flag = false;
        break;
      }

      auto [x, y, dir_pos] =
          available_positions[std::uniform_int_distribution<size_t>(0, available_positions.size() - 1)(gen)];
      for (int q = 0; q < ship_len; ++q) {
        field[x + q * DIRECTIONS[dir_pos].first][y + q * DIRECTIONS[dir_pos].second] = SHIP_CELL;
      }
    }

    if (flag) {
      break;
    }
  }
}

field_t::field_t(std::vector<std::string> field) : field(std::move(field)) {
  assert(this->field.size() == FIELD_SIZE);
  for (const auto &line : this->field) {
    assert(line.size() == FIELD_SIZE);
    assert(std::all_of(line.begin(), line.end(), [](char c) { return c == EMPTY_CELL || c == SHIP_CELL; }));
  }
}

std::ostream &operator<<(std::ostream &stream, const field_t &field) {
  stream << "    ABCDEFGHIJ" << std::endl;
  for (size_t i = 0; i < field_t::FIELD_SIZE; ++i) {
    stream << " ";
    if (i + 1 < field_t::FIELD_SIZE) {
      stream << " ";
    }
    stream << i + 1 << " ";
    stream << field[i];
    stream << std::endl;
  }
  return stream;
}
