#pragma once

#include <cassert>
#include <random>

#include "player-interface.h"

class simple_ai_player_t : public player_interface_t {
public:
  explicit simple_ai_player_t(size_t *wins_counter = nullptr);

  std::pair<int, int> make_move(const field_t &my_field, const field_t &enemy_field) override;

  void on_incorrect_move(int, int) override {
    assert(false);
  }
  void on_duplicate_move(int, int) override {
    assert(false);
  }
  void on_miss(int, int) override {}
  void on_hit(int, int) override {}
  void on_kill(int, int) override {}

  void on_win() override {
    if (wins_counter) {
      *wins_counter += 1;
    }
  }
  void on_lose() override {}

private:
  std::mt19937_64 gen;
  size_t *wins_counter{nullptr};
};
