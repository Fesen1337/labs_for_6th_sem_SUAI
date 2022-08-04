#pragma once

#include <string>
#include <vector>

class trie_naive_t {
public:
  trie_naive_t() = default;
  ~trie_naive_t() = default;

  trie_naive_t(const trie_naive_t &other) = default;
  trie_naive_t &operator=(const trie_naive_t &other) = default;

  void insert(const std::string &str);
  bool erase(const std::string &str);

  bool find(const std::string &str) const;
  size_t count_with_prefix(const std::string &prefix) const;
  std::string operator[](size_t index) const;

  std::vector<std::string> to_vector() const;

  void clear();

  size_t size() const;
  bool empty() const;

  void swap(trie_naive_t &other);

private:
  std::vector<std::string> line;
};
