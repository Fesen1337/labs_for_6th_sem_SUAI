#include "gtest/gtest.h"

#include <chrono>
#include <optional>
#include <random>
#include <sstream>
#include <vector>

#include "game/sea-battle.h"
#include "players/human-player.h"

struct action_interface_t {
  virtual std::optional<std::string> call(human_player_t &hp) const = 0;
  virtual void to_input_stream(std::ostream &) const {}
  virtual void expected_output_stream(std::ostream &) const = 0;
};

struct make_move_t : action_interface_t {
  make_move_t(int x, int y, const field_t *my_field, const field_t *enemy_field,
              std::vector<std::string> bad_lines = {})
      : x(x), y(y), my_field(my_field), enemy_field(enemy_field), bad_lines(std::move(bad_lines)) {}
  std::optional<std::string> call(human_player_t &hp) const override {
    auto [nx, ny] = hp.make_move(*my_field, *enemy_field);
    if (x != nx || y != ny) {
      return "Expected coords: {" + std::to_string(x) + ", " + std::to_string(y) + "}\tActual coords: {" +
             std::to_string(nx) + ", " + std::to_string(ny) + "}";
    }
    return std::nullopt;
  }
  void to_input_stream(std::ostream &s) const override {
    for (const auto &line : bad_lines) {
      s << line << std::endl;
    }
    s << x + 1 << static_cast<char>('A' + y) << std::endl;
  }
  void expected_output_stream(std::ostream &s) const override {
    s << "Your field:" << std::endl << *my_field << std::endl;
    s << "Enemy's field" << std::endl << *enemy_field << std::endl;
    s << "Input you turn! [Example 1A or 10B or 6J]" << std::endl;

    for (size_t i = 0; i < bad_lines.size(); ++i) {
      s << "Incorrect move! [Correct example 1A or 10B or 6J]" << std::endl;
    }
  }
  int x;
  int y;
  const field_t *my_field;
  const field_t *enemy_field;
  std::vector<std::string> bad_lines;
};

struct end_impl_t : action_interface_t {
  explicit end_impl_t(std::string str, void (human_player_t::*method)()) : str(std::move(str)), method(method) {}
  void expected_output_stream(std::ostream &s) const override {
    s << "You " << str << "!" << std::endl;
  }
  std::optional<std::string> call(human_player_t &hp) const override {
    (hp.*method)();
    return std::nullopt;
  }
  std::string str;
  void (human_player_t::*method)();
};
struct win_t : end_impl_t {
  win_t() : end_impl_t("WIN", &human_player_t::on_win) {}
};
struct lose_t : end_impl_t {
  lose_t() : end_impl_t("LOSE", &human_player_t::on_lose) {}
};

struct with_point_impl_t : action_interface_t {
  with_point_impl_t(int x, int y, std::string suffix, void (human_player_t::*method)(int, int))
      : x(x), y(y), suffix(std::move(suffix)), method(method) {}

  std::optional<std::string> call(human_player_t &hp) const override {
    (hp.*method)(x, y);
    return std::nullopt;
  }

  void expected_output_stream(std::ostream &s) const override {
    s << x + 1 << static_cast<char>('A' + y) << " " << suffix << std::endl;
  }

  int x;
  int y;
  std::string suffix;
  void (human_player_t::*method)(int, int);
};
struct duplicate_t : with_point_impl_t {
  explicit duplicate_t(int x, int y)
      : with_point_impl_t(x, y, "DUPLICATE! Repeat move!", &human_player_t::on_duplicate_move) {}
};
struct miss_t : with_point_impl_t {
  explicit miss_t(int x, int y) : with_point_impl_t(x, y, "MISS! Enemy's turn!", &human_player_t::on_miss) {}
};
struct hit_t : with_point_impl_t {
  explicit hit_t(int x, int y) : with_point_impl_t(x, y, "HIT! Your next move!", &human_player_t::on_hit) {}
};
struct kill_t : with_point_impl_t {
  explicit kill_t(int x, int y) : with_point_impl_t(x, y, "KILL! Your next move!", &human_player_t::on_kill) {}
};

static bool run_test(const std::vector<std::shared_ptr<action_interface_t>> &script) {
  std::stringstream input_stream;
  std::stringstream output_stream;

  human_player_t p(input_stream, output_stream);

  for (const auto &ptr : script) {
    std::stringstream tmp_input_stream;
    ptr->to_input_stream(tmp_input_stream);

    std::string input = tmp_input_stream.str();
    input_stream << input;

    auto call_res = ptr->call(p);
    if (call_res) {
      std::cout << "On input: " << std::endl;
      std::cout << input << std::endl;
      std::cout << "Was error: " << std::endl;
      std::cout << *call_res << std::endl << std::endl;
      return false;
    }

    std::stringstream expected_output_stream;
    ptr->expected_output_stream(expected_output_stream);

    std::string expected_output = expected_output_stream.str();
    std::string actual_output = output_stream.str();

    if (expected_output != actual_output) {
      std::cout << "On input: " << std::endl;
      std::cout << input << std::endl;

      std::cout << "EXPECTED OUTPUT is:" << std::endl;
      std::cout << expected_output << std::endl << std::endl;

      std::cout << "ACTUAL OUTPUT is:" << std::endl;
      std::cout << actual_output << std::endl << std::endl;
      return false;
    }

    output_stream = std::stringstream();
  }
  return true;
}

TEST(HumanTest, WinLoseTest) {
  ASSERT_TRUE(run_test({std::make_shared<win_t>(), std::make_shared<win_t>(), std::make_shared<lose_t>(),
                        std::make_shared<lose_t>(), std::make_shared<win_t>(), std::make_shared<lose_t>()}));
}

static std::mt19937_64 &get_gen() {
  static std::seed_seq seq = {std::chrono::high_resolution_clock::now().time_since_epoch().count()};
  static std::mt19937_64 gen(seq);
  return gen;
}

template <typename Action>
void points_test() {
  static constexpr size_t iterations = 20;
  for (size_t i = 0; i < iterations; ++i) {
    std::vector<std::shared_ptr<action_interface_t>> script;

    for (int x = 0; x < field_t::FIELD_SIZE; ++x) {
      for (int y = 0; y < field_t::FIELD_SIZE; ++y) {
        script.push_back(std::make_shared<Action>(x, y));
      }
    }
    std::shuffle(script.begin(), script.end(), get_gen());

    ASSERT_TRUE(run_test(script));
  }
}

TEST(HumanTest, DuplicateTest) {
  points_test<duplicate_t>();
}
TEST(HumanTest, MissTest) {
  points_test<miss_t>();
}
TEST(HumanTest, HitTest) {
  points_test<hit_t>();
}
TEST(HumanTest, KillTest) {
  points_test<kill_t>();
}

template <typename Action>
static void add_point_actions(size_t count, std::vector<std::shared_ptr<action_interface_t>> &script) {
  std::uniform_int_distribution<int> gen_coord(0, field_t::FIELD_SIZE - 1);
  for (size_t i = 0; i < count; ++i) {
    script.push_back(std::make_shared<Action>(gen_coord(get_gen()), gen_coord(get_gen())));
  }
}

TEST(HumanTest, ShufflePointTest) {
  std::vector<std::shared_ptr<action_interface_t>> script;

  add_point_actions<duplicate_t>(20, script);
  add_point_actions<miss_t>(20, script);
  add_point_actions<hit_t>(20, script);
  add_point_actions<kill_t>(20, script);

  std::shuffle(script.begin(), script.end(), get_gen());
  ASSERT_TRUE(run_test(script));
}

TEST(HumanTest, MakeMoveTest) {
  field_t f1(100);
  field_t f2(101);

  std::vector<std::shared_ptr<action_interface_t>> script;

  for (int x = 0; x < field_t::FIELD_SIZE; ++x) {
    for (int y = 0; y < field_t::FIELD_SIZE; ++y) {
      script.push_back(std::make_shared<make_move_t>(x, y, &f1, &f2));
    }
  }
  std::shuffle(script.begin(), script.end(), get_gen());

  ASSERT_TRUE(run_test(script));
}

TEST(HumanTest, MakeMoveIncorrectTest) {
  std::vector<std::string> bad_strings = {"",
                                          "-1A",
                                          "1 A",
                                          "A1",
                                          "A 1",
                                          "dgsldkfjglsdg",
                                          "11A",
                                          "10000000000000000000000000000000000000000A",
                                          "10000000000B",
                                          std::to_string(static_cast<long long>(std::numeric_limits<int>::max()) + 1) +
                                              "A",
                                          "1Z",
                                          "Z1",
                                          "AA",
                                          "1a",
                                          std::string(2, '\0'),
                                          "1d",
                                          "--",
                                          "++",
                                          "78",
                                          "10K",
                                          "11K",
                                          "2Z",
                                          "\t\t",
                                          ",.",
                                          "1.A",
                                          "1,A",
                                          "A,1",
                                          "1 a",
                                          "10 d",
                                          "100A",
                                          "-1-1"};

  field_t f1(100);
  field_t f2(101);

  std::vector<std::shared_ptr<action_interface_t>> script;

  for (int x = 0; x < field_t::FIELD_SIZE; ++x) {
    for (int y = 0; y < field_t::FIELD_SIZE; ++y) {
      std::shuffle(bad_strings.begin(), bad_strings.end(), get_gen());
      script.push_back(std::make_shared<make_move_t>(
          x, y, &f1, &f2,
          std::vector<std::string>(bad_strings.begin(), bad_strings.begin() + std::uniform_int_distribution<long>(
                                                                                  0, bad_strings.size())(get_gen()))));
    }
  }
  std::shuffle(script.begin(), script.end(), get_gen());

  ASSERT_TRUE(run_test(script));
}

TEST(HumanTest, RandomTest) {
  static constexpr size_t iterations = 20;
  for (size_t i = 0; i < iterations; ++i) {
    field_t f1(std::uniform_int_distribution<uint32_t>(0)(get_gen()));
    field_t f2(std::uniform_int_distribution<uint32_t>(0)(get_gen()));
    std::vector<std::shared_ptr<action_interface_t>> script;

    add_point_actions<duplicate_t>(20, script);
    add_point_actions<miss_t>(20, script);
    add_point_actions<hit_t>(20, script);
    add_point_actions<kill_t>(20, script);

    for (int x = 0; x < field_t::FIELD_SIZE; ++x) {
      for (int y = 0; y < field_t::FIELD_SIZE; ++y) {
        script.push_back(std::make_shared<make_move_t>(x, y, &f1, &f2));
      }
    }

    for (size_t j = 0; j < 20; ++j) {
      script.push_back(std::make_shared<win_t>());
      script.push_back(std::make_shared<lose_t>());
    }

    std::shuffle(script.begin(), script.end(), get_gen());
    ASSERT_TRUE(run_test(script));
  }
}
