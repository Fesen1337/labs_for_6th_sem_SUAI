#include "gtest/gtest.h"

#include <chrono>
#include <random>

#include "game/sea-battle.h"
#include "players/simple-ai-player.h"
#include "players/smart-ai-player.h"

TEST(CompetitionTest, MainTest) {
  static constexpr size_t iterations = 1000;
  static constexpr double wins_factor = 90.0 / 100.0;

  std::seed_seq seq = {std::chrono::high_resolution_clock::now().time_since_epoch().count()};
  std::mt19937_64 gen(seq);
  const auto rnd = [&gen]() { return std::uniform_int_distribution<uint32_t>(0)(gen); };

  size_t simple_wins_counter = 0;
  for (size_t i = 0; i < iterations; ++i) {
    sea_battle_t game(std::make_shared<simple_ai_player_t>(&simple_wins_counter), field_t(rnd()),
                      std::make_shared<smart_ai_player_t>(), field_t(rnd()));
    game.play();
  }

  ASSERT_GE(iterations - simple_wins_counter, wins_factor * iterations);
}
