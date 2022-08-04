#include "testing/mock-player-data.h"

#include <cassert>
#include <iostream>

using turn_t = sea_battle_t::turn_t;

mock_player_data_t::mock_player_data_t(field_t f1, field_t f2, turn_t winner_turn,
                                             std::vector<std::tuple<turn_t, action_t, std::pair<int, int>>> script,
                                             std::function<void()> assert_false)
    : responses(std::move(script)), fields{std::move(f1), std::move(f2)}, assert_false(std::move(assert_false)) {
  std::transform(responses.begin(), responses.end(), std::back_inserter(moves),
                 [](auto val) { return std::make_tuple(std::get<0>(val), std::get<2>(val)); });
  responses.emplace_back(winner_turn, WIN, INVALID_MOVE);
  responses.emplace_back(sea_battle_t::change_turn(winner_turn), LOSE, INVALID_MOVE);
}

std::pair<int, int> mock_player_data_t::get_move(turn_t turn, const field_t &cur_field,
                                                    const field_t &enemy_field) {
  auto [expected_turn, cur_nxt] = moves[moves_pos++];

  if (expected_turn != turn) {
    output_buffer << "Expected '" << sea_battle_t::get_player_name(expected_turn) << "' makes move, but actual '"
                  << sea_battle_t::get_player_name(turn) << "' ERROR!!!\n";
    dump_and_stop();
  }

  if (cur_field != fields[turn]) {
    output_buffer << "Expected current field is:\n"
                  << fields[turn] << "\n"
                  << "Actual current field is:\n"
                  << cur_field << "\nERROR!!!\n";
    dump_and_stop();
  }

  field_t expected_enemy_field = fields[1 - turn];
  for (size_t i = 0; i < field_t::FIELD_SIZE; ++i) {
    for (size_t j = 0; j < field_t::FIELD_SIZE; ++j) {
      if (expected_enemy_field[i][j] == field_t::SHIP_CELL) {
        expected_enemy_field[i][j] = field_t::EMPTY_CELL;
      }
    }
  }

  if (enemy_field != expected_enemy_field) {
    output_buffer << "Expected enemy field is:\n"
                  << expected_enemy_field << "\n"
                  << "Actual enemy field is:\n"
                  << enemy_field << "\nERROR!!!\n";
    dump_and_stop();
  }

  output_buffer << "Player '" << sea_battle_t::get_player_name(turn) << "' makes move '" << get_move_str(cur_nxt)
                << "' OK!\n";
  return cur_nxt;
}

void mock_player_data_t::check(turn_t turn, action_t action, std::pair<int, int> nxt) {
  auto expected_response = responses[responses_pos++];
  auto [expected_turn, cur_action, cur_nxt] = expected_response;

  output_buffer << "Expected response '" << get_response_str(cur_action, cur_nxt) << "' for '"
                << sea_battle_t::get_player_name(expected_turn) << "'\tActual response '"
                << get_response_str(action, nxt) << "' for '" << sea_battle_t::get_player_name(turn) << "'";

  if (expected_response != std::make_tuple(turn, action, nxt)) {
    output_buffer << " ERROR!!!\n";
    dump_and_stop();
  }

  output_buffer << " OK!\n";

  auto &ef = fields[1 - turn];
  if (action == MISS) {
    ef[nxt.first][nxt.second] = field_t::MISS_CELL;
  } else if (action == HIT || action == KILL) {
    ef[nxt.first][nxt.second] = field_t::HIT_CELL;
  }
}

void mock_player_data_t::check(turn_t turn, action_t action) {
  check(turn, action, INVALID_MOVE);
}

std::string mock_player_data_t::get_action_str(action_t action) {
  switch (action) {
  case INCORRECT:
    return "Incorrect";
  case DUPLICATE:
    return "Duplicate";
  case MISS:
    return "Miss";
  case HIT:
    return "Hit";
  case KILL:
    return "Kill";
  case WIN:
    return "Win";
  case LOSE:
    return "Lose";
  default:
    assert(false);
  }
  return "INVALID";
}

std::string mock_player_data_t::get_response_str(action_t action, std::pair<int, int> nxt) {
  std::string res = get_action_str(action);
  if (nxt != INVALID_MOVE) {
    res += " " + get_move_str(nxt);
  }
  return res;
}

std::string mock_player_data_t::get_move_str(std::pair<int, int> nxt) {
  const auto is_valid = [](int c) { return 0 <= c && c < field_t::FIELD_SIZE; };
  std::string res;
  if (!is_valid(nxt.first) || !is_valid(nxt.second)) {
    res = "**INVALID**";
  } else {
    res = std::to_string(nxt.first + 1) + static_cast<char>('A' + nxt.second);
  }
  return res + " {" + std::to_string(nxt.first) + ", " + std::to_string(nxt.second) + "}";
}

void mock_player_data_t::dump_and_stop() {
  output_buffer << "\n\n"
                << sea_battle_t::get_player_name(sea_battle_t::FIRST_PLAYER) << " field is:\n"
                << fields[0] << "\n"
                << sea_battle_t::get_player_name(sea_battle_t::SECOND_PLAYER) << " field is:\n"
                << fields[1] << "\n";
  std::cout << "\nTRACE:\n" << output_buffer.str() << std::endl;
  assert_false();
}
