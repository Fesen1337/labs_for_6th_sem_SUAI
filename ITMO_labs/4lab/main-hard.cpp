#include "gtest/gtest.h"

#include "phone-book.h"

#include <cmath>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
class hasher_t {
public:
  hasher_t() = default;

  hasher_t &add(uint64_t val) {
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    val *= m;
    val ^= val >> r;
    val *= m;

    internal_hash ^= val;
    internal_hash *= m;

    internal_hash += 0xe6546b64;
    return *this;
  }

  hasher_t &add(double val) {
    return add(*reinterpret_cast<uint64_t *>(&val));
  }

  template <typename T>
  hasher_t &add(const std::vector<T> &v) {
    add(v.size());
    for (const auto &val : v) {
      add(val);
    }
    return *this;
  }

  hasher_t &add(const std::string &s) {
    return add(std::vector<uint64_t>(s.begin(), s.end()));
  }

  hasher_t &add(const call_t &c) {
    return add("call").add(c.number).add(c.duration_s);
  }
  hasher_t &add(const user_t &u) {
    return add("user").add(u.number).add(u.name);
  }
  hasher_t &add(const user_info_t &u) {
    return add("user_info_t").add(u.user).add(u.total_call_duration_s);
  }

  uint64_t get() const {
    return internal_hash;
  }

private:
  uint64_t internal_hash{0x63a36acaf108d0db};
};

#pragma GCC diagnostic pop

class generator_t {
public:
  explicit generator_t(uint32_t seed = 0xcdf8a2dd) : seed(seed % MOD) {}

  uint32_t operator()() {
    seed = (A * seed + B) % MOD;
    return static_cast<uint32_t>(seed);
  }

private:
  static constexpr uint64_t MOD = 1e9;
  static constexpr uint64_t A = 0x6b253d97 % MOD;
  static constexpr uint64_t B = 0x468e9f20 % MOD;

  uint64_t seed{};
};

std::string gen_str(size_t min_len, size_t max_len, generator_t &gen) {
  assert(min_len <= max_len);
  size_t len = min_len + gen() % (max_len - min_len + 1);
  std::string res;
  res.reserve(len);
  for (size_t i = 0; i < len; ++i) {
    res += 'a' + gen() % ('z' - 'a' + 1);
  }
  return res;
}

TEST(Hard, CreateVeryShortUsers) {
  phone_book_t book;
  generator_t gen(7850);
  static constexpr size_t users_count = 30'000;

  for (size_t i = 0; i < users_count; ++i) {
    book.create_user(gen_str(5, 5, gen), gen_str(4, 4, gen));
  }

  hasher_t h;
  h.add(book.search_users_by_name("", users_count));
  ASSERT_EQ(h.get(), 9365524136192513293ULL);
}

TEST(Hard, CreateShortUsers) {
  phone_book_t book;
  generator_t gen(123452);
  static constexpr size_t users_count = 20'000;

  for (size_t i = 0; i < users_count; ++i) {
    book.create_user(gen_str(5, 20, gen), gen_str(5, 20, gen));
  }

  hasher_t h;
  h.add(book.search_users_by_name("", users_count));
  ASSERT_EQ(h.get(), 7265948714765389348ULL);
}

TEST(Hard, CreateLongUsers) {
  phone_book_t book;
  generator_t gen(3569834);
  static constexpr size_t users_count = 7'000;

  for (size_t i = 0; i < users_count; ++i) {
    book.create_user(gen_str(5, 20, gen), gen_str(1000, 2000, gen));
  }

  hasher_t h;
  h.add(book.search_users_by_name("", users_count));
  ASSERT_EQ(h.get(), 5001461185872910603ULL);
}

TEST(Hard, Size) {
  phone_book_t book;
  generator_t gen(703952);
  static constexpr size_t users_count = 30'000;
  hasher_t h;
  for (size_t i = 0; i < users_count; ++i) {
    book.create_user(gen_str(5, 5, gen), gen_str(4, 4, gen));
    h.add(book.size());
  }
  ASSERT_EQ(h.get(), 13060437842391114002ULL);
}

TEST(Hard, Clear) {
  phone_book_t book;
  generator_t gen(16734);
  static constexpr size_t users_count = 300;
  static constexpr size_t iterations_count = 300;
  hasher_t h;

  for (size_t i = 0; i < iterations_count; ++i) {
    for (size_t j = 0; j < users_count; ++j) {
      h.add(book.size());
      book.create_user(gen_str(5, 5, gen), gen_str(4, 4, gen));
      h.add(book.size());
    }
    book.clear();
  }

  ASSERT_EQ(h.get(), 6667970813884221767ULL);
}

TEST(Hard, AddCallsToOneUser) {
  phone_book_t book;
  generator_t gen(64723);

  book.create_user("1", "Ivan");
  static constexpr size_t calls_count = 500'000;

  for (size_t i = 0; i < calls_count; ++i) {
    book.add_call({"1", 1});
  }
  hasher_t h;
  h.add(book.get_calls(0, calls_count));
  ASSERT_EQ(h.get(), 7750873383181145339);
}

TEST(Hard, AddCallsToManyUsers) {
  phone_book_t book;
  generator_t gen(43524);

  static constexpr size_t users_count = 5000;
  std::vector<std::string> numbers(users_count);
  for (size_t i = 0; i < users_count; ++i) {
    std::string number = gen_str(5, 5, gen);
    book.create_user(number, gen_str(5, 5, gen));
    numbers[i] = std::move(number);
  }

  static constexpr size_t calls_count = 40'000;

  for (size_t i = 0; i < calls_count; ++i) {
    book.add_call({numbers[gen() % (numbers.size())], gen() % 1000 / 100.0});
  }
  hasher_t h;
  h.add(book.get_calls(0, calls_count));
  ASSERT_EQ(h.get(), 319228645396335748ULL);
}

TEST(Hard, GetCallsOneUser) {
  phone_book_t book;
  generator_t gen(674902);

  book.create_user("1", "Ivan");
  static constexpr size_t calls_count = 300'000;

  for (size_t i = 0; i < calls_count; ++i) {
    book.add_call({"1", 1});
  }
  hasher_t h;

  static constexpr size_t iterations_count = 200'000;
  for (size_t i = 0; i < iterations_count; ++i) {
    h.add(book.get_calls(gen() % calls_count, 0))
        .add(book.get_calls(gen() % calls_count, 1))
        .add(book.get_calls(gen() % calls_count, 10));
  }
  ASSERT_EQ(h.get(), 4521848841932652155ULL);
}

TEST(Hard, GetCallsManyUsers) {
  phone_book_t book;
  generator_t gen(69002953);

  static constexpr size_t users_count = 5000;
  std::vector<std::string> numbers(users_count);
  for (size_t i = 0; i < users_count; ++i) {
    std::string number = gen_str(5, 5, gen);
    book.create_user(number, gen_str(5, 5, gen));
    numbers[i] = std::move(number);
  }

  static constexpr size_t calls_count = 40'000;

  for (size_t i = 0; i < calls_count; ++i) {
    book.add_call({numbers[gen() % (numbers.size())], gen() % 1000 / 100.0});
  }

  hasher_t h;
  static constexpr size_t iterations = 40'000;
  for (size_t i = 0; i < iterations; ++i) {
    h.add(book.get_calls(gen() % calls_count, 0))
        .add(book.get_calls(gen() % calls_count, 1))
        .add(book.get_calls(gen() % calls_count, 10));
  }
  ASSERT_EQ(h.get(), 15837059985559557275ULL);
}

TEST(Hard, SearchUsersByNameEqualNames) {
  phone_book_t book;
  generator_t gen(9783003);

  static constexpr size_t users_count = 10'000;
  std::set<std::string> numbers;
  std::string name = gen_str(20, 20, gen);
  while (numbers.size() < users_count) {
    numbers.insert(gen_str(5, 7, gen));
  }
  std::vector<std::string> v_numbers(numbers.begin(), numbers.end());

  size_t pos = 0;
  const auto add_call = [&]() {
    assert(pos > 0);
    book.add_call({v_numbers[gen() % pos], static_cast<double>(gen() % 10) + 1});
  };
  const auto add_user = [&]() {
    if (pos == users_count) {
      return;
    }
    book.create_user(v_numbers[pos++], name);
  };

  hasher_t h;
  static constexpr size_t short_iterations = 5'000;
  for (size_t i = 0; i < short_iterations; ++i) {
    add_user();
    h.add(book.search_users_by_name(name.substr(0, gen() % (name.size() + 1)), gen() % 20));
    add_call();
  }

  static constexpr size_t long_iterations = 100;
  for (size_t i = 0; i < long_iterations; ++i) {
    h.add(book.search_users_by_name(name, gen() % users_count));
    add_call();
  }
  ASSERT_EQ(h.get(), 11273384186470512539ULL);
}

TEST(Hard, SearchUsersByNameShortNames) {
  phone_book_t book;
  generator_t gen(475620945);

  static constexpr size_t users_count = 10'000;
  std::set<std::string> numbers;
  while (numbers.size() < users_count) {
    numbers.insert(gen_str(5, 7, gen));
  }
  std::vector<std::string> v_numbers(numbers.begin(), numbers.end());

  for (const std::string &str : v_numbers) {
    book.create_user(str, gen_str(3, 3, gen));
  }
  size_t pos = 0;
  const auto add_call = [&]() {
    assert(pos > 0);
    book.add_call({v_numbers[gen() % pos], static_cast<double>(gen() % 10) + 1});
  };
  const auto add_user = [&]() {
    if (pos == users_count) {
      return;
    }
    book.create_user(v_numbers[pos++], gen_str(3, 3, gen));
  };

  hasher_t h;
  static constexpr size_t short_iterations = 10'000;
  for (size_t i = 0; i < short_iterations; ++i) {
    add_user();
    h.add(book.search_users_by_name(gen_str(0, 3, gen), gen() % 20));
    add_call();
  }

  static constexpr size_t long_iterations = 100;
  for (size_t i = 0; i < long_iterations; ++i) {
    h.add(book.search_users_by_name(gen_str(0, 3, gen), gen() % users_count));
    add_call();
  }
  ASSERT_EQ(h.get(), 5416822341530351832ULL);
}

TEST(Hard, SearchUsersByNameLongNames) {
  phone_book_t book;
  generator_t gen(135354204);

  static constexpr size_t users_count = 3'000;
  std::set<std::string> numbers;
  while (numbers.size() < users_count) {
    numbers.insert(gen_str(5, 7, gen));
  }
  std::vector<std::string> v_numbers(numbers.begin(), numbers.end());

  size_t pos = 0;
  const auto add_call = [&]() {
    assert(pos > 0);
    book.add_call({v_numbers[gen() % pos], static_cast<double>(gen() % 10) + 1});
  };
  const auto add_user = [&]() {
    if (pos == users_count) {
      return;
    }
    book.create_user(v_numbers[pos++], gen_str(200, 2000, gen));
  };

  hasher_t h;
  static constexpr size_t short_iterations = 3'000;
  for (size_t i = 0; i < short_iterations; ++i) {
    add_user();
    h.add(book.search_users_by_name(gen_str(0, 3, gen), gen() % 20));
    add_call();
  }

  static constexpr size_t long_iterations = 50;
  for (size_t i = 0; i < long_iterations; ++i) {
    h.add(book.search_users_by_name(gen_str(0, 3, gen), gen() % users_count));
    add_call();
  }
  ASSERT_EQ(h.get(), 16507912738829834392ULL);
}

TEST(Hard, SearchUsersByNumber) {
  phone_book_t book;
  generator_t gen(234704763);

  static constexpr size_t users_count = 10'000;
  std::set<std::string> numbers;
  while (numbers.size() < users_count) {
    numbers.insert(gen_str(6, 6, gen));
  }
  std::vector<std::string> v_numbers(numbers.begin(), numbers.end());

  size_t pos = 0;
  const auto add_user = [&]() {
    if (pos == users_count) {
      return;
    }
    book.create_user(v_numbers[pos++], gen_str(1, 1, gen));
  };
  const auto add_call = [&]() {
    assert(pos > 0);
    book.add_call({v_numbers[gen() % pos], static_cast<double>(gen() % 10) + 1});
  };

  hasher_t h;
  static constexpr size_t iterations = 20'000;
  for (size_t i = 0; i < iterations; ++i) {
    add_user();
    h.add(book.search_users_by_number(gen_str(0, 6, gen), gen() % 20));
    add_call();
  }

  static constexpr size_t long_iterations = 1000;
  for (size_t i = 0; i < long_iterations; ++i) {
    add_user();
    h.add(book.search_users_by_number(gen_str(0, 6, gen), gen() % 1000));
    add_call();
  }
  ASSERT_EQ(h.get(), 5927401743041175964ULL);
}
