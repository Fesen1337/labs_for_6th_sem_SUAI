#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
/**
 * Structure for call-record with defined equality operator and output operator
 */
struct call_t {
  std::string number;
  double duration_s{0};

  friend bool operator==(const call_t &a, const call_t &b) {
    return a.number == b.number && a.duration_s == b.duration_s;
  }
  friend std::ostream &operator<<(std::ostream &stream, const call_t &a) {
    return stream << "call_t { number: " << a.number << ";  " << "duration_s: " << a.duration_s << "; }";
  }
};

/**
 * Structure for user with defined equality operator and output operator
 */
struct user_t {
  std::string number;
  std::string name;

  friend bool operator==(const user_t &a, const user_t &b) {
    return a.number == b.number && a.name == b.name;
  }
  friend std::ostream &operator<<(std::ostream &stream, const user_t &a) {
    return stream << "user_t { number: " << a.number << ";  " << "name: " << a.name << "; }";
  }
};

/**
 * Structure for user-info with defined equality operator and output operator
 */
struct user_info_t {
  user_t user{};
  double total_call_duration_s{0};

  friend bool operator==(const user_info_t &a, const user_info_t &b) {
    return a.user == b.user && a.total_call_duration_s == b.total_call_duration_s;
  }
  friend std::ostream &operator<<(std::ostream &stream, const user_info_t &a) {
    return stream << "user_info_t { user: " << a.user << ";  " << "duration_s: " << a.total_call_duration_s << "; }";
  }
};




/**
 * Class of phone book you have to implement
 */
class phone_book_t {
public:
  /**
   * Create empty phone book
   */
  phone_book_t() = default;

  /**
   * Copy constructor
   */
  phone_book_t(const phone_book_t &other) = default;

  /**
   * Copy assignment
   */
  phone_book_t &operator=(const phone_book_t &other) = default;

  /**
   * Destructor
   */
  ~phone_book_t() = default;

  /**
   * Creates new user with specified number and name.
   * If user with specified number was already existed, method should return false and do nothing!
   * @param number -- number of new user
   * @param name -- name of new user
   * @return was user actually created
   */
  bool create_user(const std::string &number, const std::string &name);

  /**
   * Add call-history record. If user with specified number exists
   * @param call call-history record to addition
   * @return true if user with specified number exists and call-record was actually added
   */
  bool add_call(const call_t &call);

  /**
   * All calls are sorted in ORDER of their addition.
   * Return at most count call-record starts from start_pos (zero-indexed) in ORDER
   * @param start_pos -- zero-indexed start position of call-records sorted in ORDER
   * @param count -- number of call-records to return
   * @return calls[start_pos ... start_pos + count - 1]
   */
  std::vector<call_t> get_calls(size_t start_pos, size_t count) const;

  /**
   * Find at most count users with number starts with number_prefix sorted by:
   *    total call duration
   *    name
   *    number
   * @param number_prefix prefix for users' number to search
   * @param count desired number of users to find
   * @return vector of search result, sorted by rules above
   */
  std::vector<user_info_t> search_users_by_number(const std::string &number_prefix, size_t count) const;

  /**
   * Find at most count users with name starts with name_prefix sorted by:
   *    name
   *    total call duration
   *    number
   * @param name_prefix prefix for users' name to search
   * @param count desired number of users to find
   * @return vector of search result, sorted by rules above
   */
  std::vector<user_info_t> search_users_by_name(const std::string &name_prefix, size_t count) const;

  /**
   * Make your phone book empty
   */
  void clear();

  /**
   * @return count of users in your phone book
   */
  size_t size() const;

  /**
   * @return is you phone book empty
   */
  bool empty() const;

private:

  std::multimap<std::string, std::pair<std::string,double> >users;
  std::vector<call_t>calls;

  std::multimap<std::string, std::pair<double, std::string> >users2;
};
