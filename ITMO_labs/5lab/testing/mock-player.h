#pragma once

#include <functional>
#include <string>
#include <vector>

#include "players/player-interface.h"
#include "testing/mock-player-data.h"

class mock_player_t : public player_interface_t {
public:
  mock_player_t(sea_battle_t::turn_t turn, std::shared_ptr<mock_player_data_t> data_ptr)
      : turn(turn), data_ptr(std::move(data_ptr)) {}

  std::pair<int, int> make_move(const field_t &my_field, const field_t &enemy_field) override;

  void on_incorrect_move(int x, int y) override;
  void on_duplicate_move(int x, int y) override;
  void on_miss(int x, int y) override;
  void on_hit(int x, int y) override;
  void on_kill(int x, int y) override;

  void on_win() override;
  void on_lose() override;

private:
  const sea_battle_t::turn_t turn;
  std::shared_ptr<mock_player_data_t> data_ptr;
};
