#include "gtest/gtest.h"

#include <random>

#include "trie-naive.h"
#include "trie.h"

template <typename T = int>
static T rnd(T min_val, T max_val) {
  static std::mt19937_64 gen(321);
  return std::uniform_int_distribution<T>(min_val, max_val)(gen);
}

#define FULL_COMPARE(actual, expected)                                         \
  do {                                                                         \
    ASSERT_EQ(actual.size(), expected.size());                                 \
    ASSERT_EQ(actual.empty(), expected.empty());                               \
                                                                               \
    size_t count = expected.size();                                            \
    for (size_t i = 0; i < count; ++i) {                                       \
      ASSERT_EQ(actual[i], expected[i]);                                       \
      ASSERT_TRUE(actual.find(expected[i]));                                   \
      ASSERT_TRUE(expected.find(expected[i]));                                 \
    }                                                                          \
    ASSERT_EQ(actual.to_vector(), expected.to_vector());                       \
    ASSERT_EQ(actual.count_with_prefix(""), expected.count_with_prefix(""));   \
  } while (false)

#define ERASE(actual, expected, str)                                           \
  ASSERT_EQ(actual.erase(str), expected.erase(str))

#define INSERT(actual, expected, str)                                          \
  actual.insert(str);                                                          \
  expected.insert(str)

#define HEADER_TEST(actual, expected, ...)                                     \
  trie_t actual;                                                               \
  trie_naive_t expected;                                                       \
  do {                                                                         \
    std::vector<std::string> __to_insert = {__VA_ARGS__};                      \
    for (const auto &str : __to_insert) {                                      \
      actual.insert(str);                                                      \
      expected.insert(str);                                                    \
      FULL_COMPARE(actual, expected);                                          \
    }                                                                          \
  } while (false)

#define SIMPLE_TEST(...) HEADER_TEST(actual, expected, ##__VA_ARGS__)

TEST(Trie, EmptyTrie) {
  SIMPLE_TEST();
}

TEST(Trie, OneString) {
  SIMPLE_TEST("abacaba");
}

TEST(Trie, EmptyString) {
  SIMPLE_TEST("");
}

TEST(Trie, PrefixTest) {
  SIMPLE_TEST("a", "aaaaaa", "aaaa", "aba", "abac", "aaaa", "a", "a");
  ASSERT_EQ(actual.count_with_prefix(""), expected.count_with_prefix(""));
  ASSERT_EQ(actual.count_with_prefix("a"), expected.count_with_prefix("a"));
  ASSERT_EQ(actual.find("a"), expected.find("a"));
  ASSERT_EQ(actual.find("aba"), expected.find("aba"));
}

TEST(Trie, CopyConstructorAndAssignments) {
  SIMPLE_TEST("a", "b", "a", "abacaba");
  trie_t actual_copy = actual;
  trie_naive_t expected_copy = expected;
  FULL_COMPARE(actual_copy, actual);
  FULL_COMPARE(expected_copy, expected);
  FULL_COMPARE(actual_copy, expected_copy);

  ERASE(actual, expected, "a");
  ASSERT_EQ(3, actual_copy.count_with_prefix("a"));
  ASSERT_EQ(2, actual.count_with_prefix("a"));

  actual_copy = actual_copy;
  expected_copy = expected_copy;
  FULL_COMPARE(actual_copy, expected_copy);

  actual_copy = trie_t();
  expected_copy = trie_naive_t();
  FULL_COMPARE(actual_copy, expected_copy);

  actual_copy = actual;
  expected_copy = expected;
  FULL_COMPARE(actual_copy, actual);
  FULL_COMPARE(expected_copy, expected);
  FULL_COMPARE(actual_copy, expected_copy);
  FULL_COMPARE(actual, expected);

  INSERT(actual, expected, "a");
  ASSERT_EQ(2, actual_copy.count_with_prefix("a"));
  ASSERT_EQ(3, actual.count_with_prefix("a"));
}

TEST(Trie, EraseAndInsert) {
  trie_t actual;
  trie_naive_t expected;

  ERASE(actual, expected, "a");

  INSERT(actual, expected, "");
  ERASE(actual, expected, "");

  INSERT(actual, expected, "aaaa");
  INSERT(actual, expected, "aaaa");
  ERASE(actual, expected, "aaaa");
  ERASE(actual, expected, "aaa");
  ERASE(actual, expected, "aaaaa");
  ERASE(actual, expected, "aaaa");
}

TEST(Trie, RandomEraseAndInsert) {
  trie_t actual;
  trie_naive_t expected;
  std::vector<std::string> strs = {"",     "a",    "aba",  "caba",
                                   "baba", "zaba", "zzzz", "daba"};

  size_t iterations = 1000;
  for (size_t i = 0; i < iterations; ++i) {
    bool need_insert = rnd(0, 1) == 0;
    std::string key = strs[rnd(0UL, strs.size() - 1)];
    if (need_insert) {
      INSERT(actual, expected, key);
    } else {
      ERASE(actual, expected, key);
    }
  }
}

TEST(Trie, Clear) {
  SIMPLE_TEST("", "a", "aba", "caba", "baba", "zaba", "zzzz", "daba");
  actual.clear();
  expected.clear();
  FULL_COMPARE(actual, expected);
}

TEST(Trie, Swap) {
  SIMPLE_TEST("a", "b", "aaaa", "");
  HEADER_TEST(actual2, expected2, "x", "y", "xxxx");

  actual.swap(actual2);
  expected.swap(expected2);

  FULL_COMPARE(actual, expected);
  FULL_COMPARE(actual2, expected2);
}

TEST(Trie, StressTest) {
  trie_t actual;

  const auto gen_str = [](size_t len) {
    std::string res;
    std::generate_n(std::back_inserter(res), len,
                    []() { return 'a' + rnd(0, 1); });
    return res;
  };

  size_t iterations = 100000;
  size_t expected_hash = 0;
  for (size_t i = 0; i < iterations; ++i) {
    std::string key = gen_str(rnd(0, 30));
    actual.insert(key);
    expected_hash += std::hash<std::string>()(key);
  }

  size_t actual_hash = 0;
  for (size_t i = 0; i < iterations; ++i) {
    actual_hash += std::hash<std::string>()(actual[i]);
  }

  ASSERT_EQ(expected_hash, actual_hash);
}
