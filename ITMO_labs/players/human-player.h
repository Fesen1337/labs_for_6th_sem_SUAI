#pragma once

#include <iostream>
#include <cassert>
#include "players/player-interface.h"

class human_player_t : public player_interface_t {
public:
  human_player_t(std::istream &input_stream, std::ostream &output_stream)
      : input_stream(input_stream), output_stream(output_stream) {}

  std::pair<int, int> make_move(const field_t &my_field, const field_t &enemy_field);

  void on_incorrect_move(int, int) override {
    assert(false);
  }
  void on_duplicate_move(int, int);
  void on_miss(int, int);
  void on_hit(int, int);
  void on_kill(int, int);

  void on_win();
  void on_lose();

private:
  size_t *wins_counter{nullptr};
  std::istream &input_stream;
  std::ostream &output_stream;
};
