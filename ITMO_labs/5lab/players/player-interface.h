#pragma once

#include "game/field.h"

class player_interface_t {
public:
  virtual std::pair<int, int> make_move(const field_t &my_field, const field_t &enemy_field) = 0;

  virtual void on_incorrect_move(int x, int y) = 0;
  virtual void on_duplicate_move(int x, int y) = 0;
  virtual void on_miss(int x, int y) = 0;
  virtual void on_hit(int x, int y) = 0;
  virtual void on_kill(int x, int y) = 0;

  virtual void on_win() = 0;
  virtual void on_lose() = 0;
};
