#include "trie-naive.h"

#include <cassert>

void trie_naive_t::insert(const std::string &str) {
  auto it = std::lower_bound(line.begin(), line.end(), str);
  line.insert(it, str);
}

bool trie_naive_t::erase(const std::string &str) {
  auto it = std::lower_bound(line.begin(), line.end(), str);
  if (it != line.end() && *it == str) {
    line.erase(it);
    return true;
  }
  return false;
}

bool trie_naive_t::find(const std::string &str) const {
  auto it = std::lower_bound(line.begin(), line.end(), str);
  return it != line.end() && *it == str;
}

size_t trie_naive_t::count_with_prefix(const std::string &prefix) const {
  if (prefix.empty()) {
    return size();
  }
  auto start = std::lower_bound(line.begin(), line.end(), prefix);

  std::string next_prefix = prefix;
  next_prefix.back() += 1;
  auto finish = std::lower_bound(line.begin(), line.end(), next_prefix);
  return finish - start;
}

std::string trie_naive_t::operator[](size_t index) const {
  assert(index < size());
  return line[index];
}

std::vector<std::string> trie_naive_t::to_vector() const {
  return line;
}

void trie_naive_t::clear() {
  line.clear();
}

size_t trie_naive_t::size() const {
  return line.size();
}

bool trie_naive_t::empty() const {
  return size() == 0;
}

void trie_naive_t::swap(trie_naive_t &other) {
  std::swap(line, other.line);
}
