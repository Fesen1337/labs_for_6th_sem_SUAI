#include "players/simple-ai-player.h"

#include <chrono>

simple_ai_player_t::simple_ai_player_t(size_t *wins_counter) : wins_counter(wins_counter) {
  std::seed_seq seq = {std::chrono::high_resolution_clock::now().time_since_epoch().count()};
  gen = std::mt19937_64(seq);
}

std::pair<int, int> simple_ai_player_t::make_move(const field_t &, const field_t &enemy_field) {
  std::vector<std::pair<int, int>> empty_cells;
  for (int i = 0; i < field_t::FIELD_SIZE; ++i) {
    for (int j = 0; j < field_t::FIELD_SIZE; ++j) {
      if (enemy_field[i][j] == field_t::EMPTY_CELL) {
        empty_cells.emplace_back(i, j);
      }
    }
  }
  assert(!empty_cells.empty());
  return empty_cells[std::uniform_int_distribution<int>(0, empty_cells.size() - 1)(gen)];
}
