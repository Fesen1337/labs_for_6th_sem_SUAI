#include "gtest/gtest.h"

#include <thread>

#include "game/sea-battle.h"
#include "testing/mock-player-data.h"
#include "testing/mock-player.h"

using turn_t = sea_battle_t::turn_t;

static void run_test(field_t f1, field_t f2, turn_t winner_turn,
                     std::vector<std::tuple<turn_t, mock_player_data_t::action_t, std::pair<int, int>>> script) {
  auto test_data =
      std::make_shared<mock_player_data_t>(f1, f2, winner_turn, std::move(script), []() { ASSERT_TRUE(false); });
  auto first = std::make_shared<mock_player_t>(turn_t::FIRST_PLAYER, test_data);
  auto second = std::make_shared<mock_player_t>(turn_t::SECOND_PLAYER, test_data);

  sea_battle_t game(std::move(first), std::move(f1), std::move(second), std::move(f2));
  game.play();
}

TEST(GameTest, OneShipTest) {
  field_t f1 = {"#.........", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};
  field_t f2 = {"#.........", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};

  std::vector<std::tuple<turn_t, mock_player_data_t::action_t, std::pair<int, int>>> script = {
      {turn_t::FIRST_PLAYER, mock_player_data_t::KILL, {0, 0}}};

  run_test(f1, f2, turn_t::FIRST_PLAYER, script);
}

TEST(GameTest, SimpleMissTest) {
  field_t f1 = {"#.........", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};
  field_t f2 = {"#.........", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};

  std::vector<std::tuple<turn_t, mock_player_data_t::action_t, std::pair<int, int>>> script = {
      {turn_t::FIRST_PLAYER, mock_player_data_t::MISS, {1, 0}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::KILL, {0, 0}},
  };

  run_test(f1, f2, turn_t::SECOND_PLAYER, script);
}

TEST(GameTest, MultipleHitsTest) {
  field_t f1 = {"####......", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};
  field_t f2 = {"####......", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};

  std::vector<std::tuple<turn_t, mock_player_data_t::action_t, std::pair<int, int>>> script = {
      {turn_t::FIRST_PLAYER, mock_player_data_t::HIT, {0, 3}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::HIT, {0, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::HIT, {0, 1}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::KILL, {0, 2}},
  };

  run_test(f1, f2, turn_t::FIRST_PLAYER, script);
}

TEST(GameTest, MultipleHitsSecondTest) {
  field_t f1 = {"####......", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};
  field_t f2 = {"####......", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};

  std::vector<std::tuple<turn_t, mock_player_data_t::action_t, std::pair<int, int>>> script = {
      {turn_t::FIRST_PLAYER, mock_player_data_t::HIT, {0, 3}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::HIT, {0, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::HIT, {0, 1}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::MISS, {0, 4}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::HIT, {0, 3}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::HIT, {0, 0}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::HIT, {0, 1}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::KILL, {0, 2}},
  };

  run_test(std::move(f1), std::move(f2), turn_t::SECOND_PLAYER, script);
}

TEST(GameTest, IncorrectMovesTest) {
  field_t f1 = {"#.........", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};
  field_t f2 = {"#.........", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};

  std::vector<std::tuple<turn_t, mock_player_data_t::action_t, std::pair<int, int>>> script = {
      {turn_t::FIRST_PLAYER, mock_player_data_t::INCORRECT, {-1, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::INCORRECT, {-2, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::INCORRECT, {10, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::INCORRECT, {11, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::INCORRECT, {0, -1}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::INCORRECT, {0, 10}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::INCORRECT, {-1, 10}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::INCORRECT, {10, -1}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::KILL, {0, 0}}};

  run_test(std::move(f1), std::move(f2), turn_t::FIRST_PLAYER, script);
}

TEST(GameTest, DuplicateMovesTest) {
  field_t f1 = {"#.........", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};
  field_t f2 = {"###.......", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};

  std::vector<std::tuple<turn_t, mock_player_data_t::action_t, std::pair<int, int>>> script = {
      {turn_t::FIRST_PLAYER, mock_player_data_t::HIT, {0, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::DUPLICATE, {0, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::HIT, {0, 2}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::DUPLICATE, {0, 0}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::DUPLICATE, {0, 2}},
      {turn_t::FIRST_PLAYER, mock_player_data_t::KILL, {0, 1}},
  };

  run_test(std::move(f1), std::move(f2), turn_t::FIRST_PLAYER, script);
}

TEST(GameTest, MultipleShipsTest) {
  field_t f1 = {"#..#......", "#..#......", "#.........", "#.........", "..........",
                "..........", "..........", "..........", ".........#", ".........#"};
  field_t f2 = {"#.........", "..........", "..........", "..........", "..........",
                "..........", "..........", "..........", "..........", ".........."};

  std::vector<std::tuple<turn_t, mock_player_data_t::action_t, std::pair<int, int>>> script = {
      {turn_t::FIRST_PLAYER, mock_player_data_t::MISS, {0, 1}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::HIT, {0, 0}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::HIT, {9, 9}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::HIT, {0, 3}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::KILL, {8, 9}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::HIT, {3, 0}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::HIT, {2, 0}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::KILL, {1, 3}},
      {turn_t::SECOND_PLAYER, mock_player_data_t::KILL, {1, 0}},
  };

  run_test(std::move(f1), std::move(f2), turn_t::SECOND_PLAYER, script);
}

TEST(GameTest, SimpleSimpleTest){
#include "testing/simple-simple.test"
}

TEST(GameTest, SimpleSmartTest){
#include "testing/simple-smart.test"
}

TEST(GameTest, SmartSimpleTest){
#include "testing/smart-simple.test"
}

TEST(GameTest, SmartSmartTest) {
#include "testing/smart-smart.test"
}

