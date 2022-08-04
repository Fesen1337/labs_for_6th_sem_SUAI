#include "testing/mock-player.h"

std::pair<int, int> mock_player_t::make_move(const field_t &my_field, const field_t &enemy_field) {
  return data_ptr->get_move(turn, my_field, enemy_field);
}

void mock_player_t::on_incorrect_move(int x, int y) {
  data_ptr->check(turn, mock_player_data_t::INCORRECT, {x, y});
}
void mock_player_t::on_duplicate_move(int x, int y) {
  data_ptr->check(turn, mock_player_data_t::DUPLICATE, {x, y});
}
void mock_player_t::on_miss(int x, int y) {
  data_ptr->check(turn, mock_player_data_t::MISS, {x, y});
}
void mock_player_t::on_hit(int x, int y) {
  data_ptr->check(turn, mock_player_data_t::HIT, {x, y});
}
void mock_player_t::on_kill(int x, int y) {
  data_ptr->check(turn, mock_player_data_t::KILL, {x, y});
}

void mock_player_t::on_win() {
  data_ptr->check(turn, mock_player_data_t::WIN);
}
void mock_player_t::on_lose() {
  data_ptr->check(turn, mock_player_data_t::LOSE);
}
