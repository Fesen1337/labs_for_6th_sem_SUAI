#pragma once

#include <array>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <sstream>
#include <vector>

#include "game/field.h"
#include "game/sea-battle.h"

class mock_player_data_t {
public:
  enum action_t {
    INCORRECT,
    DUPLICATE,
    MISS,
    HIT,
    KILL,

    WIN,
    LOSE
  };

  explicit mock_player_data_t(field_t f1, field_t f2, sea_battle_t::turn_t winner_turn,
                                 std::vector<std::tuple<sea_battle_t::turn_t, action_t, std::pair<int, int>>> script,
                                 std::function<void()> assert_false);

  std::pair<int, int> get_move(sea_battle_t::turn_t turn, const field_t &cur_field, const field_t &enemy_field);

  void check(sea_battle_t::turn_t turn, action_t action, std::pair<int, int> nxt);
  void check(sea_battle_t::turn_t turn, action_t action);

private:
  static std::string get_move_str(std::pair<int, int> nxt);
  static std::string get_action_str(action_t action);
  static std::string get_response_str(action_t action, std::pair<int, int> nxt);

  void dump_and_stop();

  std::stringstream output_buffer;

  size_t moves_pos{0};
  std::vector<std::tuple<sea_battle_t::turn_t, std::pair<int, int>>> moves;
  size_t responses_pos{0};
  std::vector<std::tuple<sea_battle_t::turn_t, action_t, std::pair<int, int>>> responses;

  std::array<field_t, 2> fields;
  std::function<void()> assert_false;

  static constexpr std::pair<int, int> INVALID_MOVE = {-1, -1};
};
