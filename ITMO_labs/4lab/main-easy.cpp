#include "gtest/gtest.h"

#include "phone-book.h"
#include "utils.h"


TEST(Easy, SimpleTest) {
  phone_book_t book;

  ASSERT_FALSE(book.add_call({"123", 0}));

  ASSERT_TRUE(book.create_user("123", "Ivan"));
  ASSERT_TRUE(book.add_call({"123", 1}));
  ASSERT_FALSE(book.add_call({"321", 2}));
}

TEST(Easy, CreateUsers) {
  phone_book_t book;

  ASSERT_TRUE(book.create_user("123", "Ivan"));
  ASSERT_FALSE(book.create_user("123", "Ivan"));
  ASSERT_FALSE(book.create_user("123", "Anton"));

  ASSERT_TRUE(book.create_user("1", "Ivan"));
  ASSERT_FALSE(book.create_user("1", "Anna"));

  ASSERT_TRUE(book.create_user("42521", "sdfjhglksdlksjdflgskdjfhgksdjflhgksjdgdjfhglskjdhbkjhdslkfjg"));
}

TEST(Easy, Calls) {
  phone_book_t book;

  ASSERT_EQ(book.get_calls(0, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(0, 20), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(10, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(10, 20), std::vector<call_t>());

  ASSERT_TRUE(book.create_user("123", "Ivan"));
  ASSERT_TRUE(book.create_user("321", "Anton"));

  ASSERT_EQ(book.get_calls(0, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(0, 20), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(10, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(10, 20), std::vector<call_t>());

  ASSERT_TRUE(book.add_call({"123", 10}));

  ASSERT_EQ(book.get_calls(0, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(0, 20), std::vector<call_t>({{"123", 10}}));
  ASSERT_EQ(book.get_calls(10, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(10, 20), std::vector<call_t>());

  ASSERT_FALSE(book.add_call({"1", 10}));

  ASSERT_EQ(book.get_calls(0, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(0, 20), std::vector<call_t>({{"123", 10}}));
  ASSERT_EQ(book.get_calls(10, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(10, 20), std::vector<call_t>());

  ASSERT_FALSE(book.add_call({"1234", 10}));

  ASSERT_EQ(book.get_calls(0, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(0, 20), std::vector<call_t>({{"123", 10}}));
  ASSERT_EQ(book.get_calls(10, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(10, 20), std::vector<call_t>());

  ASSERT_TRUE(book.add_call({"321", 10}));

  ASSERT_EQ(book.get_calls(0, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(0, 20), std::vector<call_t>({{"123", 10}, {"321", 10}}));
  ASSERT_EQ(book.get_calls(1, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(1, 20), std::vector<call_t>({{"321", 10}}));

  ASSERT_TRUE(book.add_call({"123", 9}));
  ASSERT_TRUE(book.add_call({"123", 2}));
  ASSERT_TRUE(book.add_call({"321", 7}));

  ASSERT_EQ(book.get_calls(0, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(0, 1), std::vector<call_t>({{"123", 10}}));
  ASSERT_EQ(book.get_calls(0, 2), std::vector<call_t>({{"123", 10}, {"321", 10}}));
  ASSERT_EQ(book.get_calls(0, 5), std::vector<call_t>({{"123", 10}, {"321", 10}, {"123", 9}, {"123", 2}, {"321", 7}}));
  ASSERT_EQ(book.get_calls(0, 100),
            std::vector<call_t>({{"123", 10}, {"321", 10}, {"123", 9}, {"123", 2}, {"321", 7}}));

  ASSERT_EQ(book.get_calls(2, 0), std::vector<call_t>());
  ASSERT_EQ(book.get_calls(2, 1), std::vector<call_t>({{"123", 9}}));
  ASSERT_EQ(book.get_calls(2, 2), std::vector<call_t>({{"123", 9}, {"123", 2}}));
  ASSERT_EQ(book.get_calls(2, 3), std::vector<call_t>({{"123", 9}, {"123", 2}, {"321", 7}}));
  ASSERT_EQ(book.get_calls(2, 5), std::vector<call_t>({{"123", 9}, {"123", 2}, {"321", 7}}));
  ASSERT_EQ(book.get_calls(2, 100), std::vector<call_t>({{"123", 9}, {"123", 2}, {"321", 7}}));
}


#define CHECK_SEARCH_USERS_BY_ORDER_COUNT(method, prefix, order, count)                                                \
  ASSERT_EQ(book.method(prefix, count),                                                                                \
            std::vector<user_info_t>(order.begin(), order.begin() + std::min<size_t>(count, order.size())))

#define CHECK_SEARCH_USERS_BY_ORDER(method, prefix, order)                                                             \
  CHECK_SEARCH_USERS_BY_ORDER_COUNT(method, prefix, order, 0);                                                         \
  CHECK_SEARCH_USERS_BY_ORDER_COUNT(method, prefix, order, 1);                                                         \
  CHECK_SEARCH_USERS_BY_ORDER_COUNT(method, prefix, order, 2);                                                         \
  CHECK_SEARCH_USERS_BY_ORDER_COUNT(method, prefix, order, 3);                                                         \
  CHECK_SEARCH_USERS_BY_ORDER_COUNT(method, prefix, order, order.size());                                              \
  CHECK_SEARCH_USERS_BY_ORDER_COUNT(method, prefix, order, order.size() + 1);                                          \
  CHECK_SEARCH_USERS_BY_ORDER_COUNT(method, prefix, order, 100)

#define CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER(prefix, order)                                                           \
  CHECK_SEARCH_USERS_BY_ORDER(search_users_by_number, prefix, order)

TEST(Easy, SearchUsersByNumberDefault) {
  phone_book_t book;

  ASSERT_TRUE(book.create_user("123", "Ivan"));
  ASSERT_TRUE(book.create_user("12", "Anton"));
  ASSERT_TRUE(book.create_user("1", "Pavel"));
  ASSERT_TRUE(book.create_user("", "Pavel"));
  ASSERT_TRUE(book.create_user("1234", "Ivan"));
  ASSERT_TRUE(book.create_user("12345", "Ivan"));
  ASSERT_TRUE(book.create_user("124", "Nikita"));
  ASSERT_TRUE(book.create_user("1244", "Nikita"));
  ASSERT_TRUE(book.create_user("2", "Oleg"));

  std::vector<user_info_t> empty_prefix_default_order = {
      {{"12", "Anton"}, 0},   {{"123", "Ivan"}, 0},   {{"1234", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0}, {{"124", "Nikita"}, 0}, {{"1244", "Nikita"}, 0},
      {{"2", "Oleg"}, 0},     {{"", "Pavel"}, 0},     {{"1", "Pavel"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("", empty_prefix_default_order);

  std::vector<user_info_t> one_prefix_default_order = {
      {{"12", "Anton"}, 0},   {{"123", "Ivan"}, 0},    {{"1234", "Ivan"}, 0}, {{"12345", "Ivan"}, 0},
      {{"124", "Nikita"}, 0}, {{"1244", "Nikita"}, 0}, {{"1", "Pavel"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("1", one_prefix_default_order);

  std::vector<user_info_t> one_one_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("11", one_one_prefix_default_order);

  std::vector<user_info_t> one_two_prefix_default_order = {
      {{"12", "Anton"}, 0},   {{"123", "Ivan"}, 0},   {{"1234", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0}, {{"124", "Nikita"}, 0}, {{"1244", "Nikita"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("12", one_two_prefix_default_order);

  std::vector<user_info_t> one_two_three_prefix_default_order = {
      {{"123", "Ivan"}, 0},
      {{"1234", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("123", one_two_three_prefix_default_order);

  std::vector<user_info_t> one_two_three_four_five_prefix_default_order = {
      {{"12345", "Ivan"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("12345", one_two_three_four_five_prefix_default_order);

  std::vector<user_info_t> one_two_three_four_five_six_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("123456", one_two_three_four_five_six_prefix_default_order);
}

TEST(Easy, SearchUsersByNumberWithDurations) {
  phone_book_t book;

  ASSERT_TRUE(book.create_user("123", "Ivan"));
  ASSERT_TRUE(book.create_user("12", "Anton"));
  ASSERT_TRUE(book.create_user("1", "Pavel"));
  ASSERT_TRUE(book.create_user("", "Pavel"));
  ASSERT_TRUE(book.create_user("1234", "Ivan"));
  ASSERT_TRUE(book.create_user("12345", "Ivan"));
  ASSERT_TRUE(book.create_user("124", "Nikita"));
  ASSERT_TRUE(book.create_user("1244", "Nikita"));
  ASSERT_TRUE(book.create_user("2", "Oleg"));
  ASSERT_FALSE(book.create_user("2", "NotOleg"));

  ASSERT_TRUE(book.add_call({"2", 10}));
  ASSERT_TRUE(book.add_call({"1234", 5}));
  ASSERT_TRUE(book.add_call({"1244", 3}));
  ASSERT_TRUE(book.add_call({"", 1}));
  ASSERT_TRUE(book.add_call({"", 1}));
  ASSERT_TRUE(book.add_call({"1244", 2}));
  ASSERT_TRUE(book.add_call({"", 1}));

  std::vector<user_info_t> empty_prefix_default_order = {
      {{"2", "Oleg"}, 10},  {{"1234", "Ivan"}, 5},  {{"1244", "Nikita"}, 5}, {{"", "Pavel"}, 3},  {{"12", "Anton"}, 0},
      {{"123", "Ivan"}, 0}, {{"12345", "Ivan"}, 0}, {{"124", "Nikita"}, 0},  {{"1", "Pavel"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("", empty_prefix_default_order);

  std::vector<user_info_t> one_prefix_default_order = {
      {{"1234", "Ivan"}, 5},  {{"1244", "Nikita"}, 5}, {{"12", "Anton"}, 0}, {{"123", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0}, {{"124", "Nikita"}, 0},  {{"1", "Pavel"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("1", one_prefix_default_order);

  std::vector<user_info_t> one_one_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("11", one_one_prefix_default_order);

  std::vector<user_info_t> one_two_prefix_default_order = {
      {{"1234", "Ivan"}, 5}, {{"1244", "Nikita"}, 5}, {{"12", "Anton"}, 0},
      {{"123", "Ivan"}, 0},  {{"12345", "Ivan"}, 0},  {{"124", "Nikita"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("12", one_two_prefix_default_order);

  std::vector<user_info_t> one_two_three_prefix_default_order = {
      {{"1234", "Ivan"}, 5},
      {{"123", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("123", one_two_three_prefix_default_order);

  std::vector<user_info_t> one_two_three_four_five_prefix_default_order = {
      {{"12345", "Ivan"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("12345", one_two_three_four_five_prefix_default_order);

  std::vector<user_info_t> one_two_three_four_five_six_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("123456", one_two_three_four_five_six_prefix_default_order);

  ASSERT_TRUE(book.add_call({"123", 7}));
  ASSERT_FALSE(book.add_call({"7", 8}));
  ASSERT_TRUE(book.add_call({"12", 6}));
  ASSERT_TRUE(book.add_call({"1", 3}));
  ASSERT_TRUE(book.add_call({"", 12}));
  ASSERT_TRUE(book.add_call({"1234", 20}));
  ASSERT_TRUE(book.add_call({"12345", 7}));
  ASSERT_TRUE(book.add_call({"124", 12}));
  ASSERT_TRUE(book.add_call({"1244", 22}));
  ASSERT_TRUE(book.add_call({"2", 17}));

  std::vector<user_info_t> changed_empty_prefix_default_order = {
      {{"1244", "Nikita"}, 27}, {{"2", "Oleg"}, 27},     {{"1234", "Ivan"}, 25},
      {{"", "Pavel"}, 15},      {{"124", "Nikita"}, 12}, {{"123", "Ivan"}, 7},
      {{"12345", "Ivan"}, 7},   {{"12", "Anton"}, 6},    {{"1", "Pavel"}, 3},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("", changed_empty_prefix_default_order);

  std::vector<user_info_t> changed_one_prefix_default_order = {
      {{"1244", "Nikita"}, 27}, {{"1234", "Ivan"}, 25}, {{"124", "Nikita"}, 12}, {{"123", "Ivan"}, 7},
      {{"12345", "Ivan"}, 7},   {{"12", "Anton"}, 6},   {{"1", "Pavel"}, 3},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("1", changed_one_prefix_default_order);

  std::vector<user_info_t> changed_one_one_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("11", changed_one_one_prefix_default_order);

  std::vector<user_info_t> changed_one_two_prefix_default_order = {
      {{"1244", "Nikita"}, 27}, {{"1234", "Ivan"}, 25}, {{"124", "Nikita"}, 12},
      {{"123", "Ivan"}, 7},     {{"12345", "Ivan"}, 7}, {{"12", "Anton"}, 6},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("12", changed_one_two_prefix_default_order);

  std::vector<user_info_t> changed_one_two_three_prefix_default_order = {
      {{"1234", "Ivan"}, 25},
      {{"123", "Ivan"}, 7},
      {{"12345", "Ivan"}, 7},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("123", changed_one_two_three_prefix_default_order);

  std::vector<user_info_t> changed_one_two_three_four_five_prefix_default_order = {
      {{"12345", "Ivan"}, 7},
  };
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("12345", changed_one_two_three_four_five_prefix_default_order);

  std::vector<user_info_t> changed_one_two_three_four_five_six_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER("123456", changed_one_two_three_four_five_six_prefix_default_order);
}

#undef CHECK_SEARCH_USERS_BY_NUMBER_BY_ORDER

#define CHECK_SEARCH_USERS_BY_NAME_BY_ORDER(prefix, order)                                                             \
  CHECK_SEARCH_USERS_BY_ORDER(search_users_by_name, prefix, order)

TEST(Easy, SearchUsersByNameDefault) {
  phone_book_t book;

  ASSERT_TRUE(book.create_user("123", "Ivan"));
  ASSERT_TRUE(book.create_user("12", "Ivan"));
  ASSERT_TRUE(book.create_user("1", "Iven"));
  ASSERT_TRUE(book.create_user("", "Ivbn"));
  ASSERT_TRUE(book.create_user("1234", "Ivan"));
  ASSERT_TRUE(book.create_user("12345", "Ivan"));
  ASSERT_TRUE(book.create_user("124", "Nikita"));
  ASSERT_TRUE(book.create_user("1244", "Nikita"));
  ASSERT_TRUE(book.create_user("2", "Bleg"));

  std::vector<user_info_t> empty_prefix_default_order = {
      {{"2", "Bleg"}, 0},
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"1234", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
      {{"", "Ivbn"}, 0},
      {{"1", "Iven"}, 0},
      {{"124", "Nikita"}, 0},
      {{"1244", "Nikita"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("", empty_prefix_default_order);

  std::vector<user_info_t> I_prefix_default_order = {
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"1234", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
      {{"", "Ivbn"}, 0},
      {{"1", "Iven"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("I", I_prefix_default_order);

  std::vector<user_info_t> D_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("D", D_prefix_default_order);

  std::vector<user_info_t> Iv_prefix_default_order = {
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"1234", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
      {{"", "Ivbn"}, 0},
      {{"1", "Iven"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Iv", Iv_prefix_default_order);

  std::vector<user_info_t> Iva_prefix_default_order = {
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"1234", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Iva", Iva_prefix_default_order);

  std::vector<user_info_t> Ivan_prefix_default_order = {
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"1234", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Ivan", Ivan_prefix_default_order);

  std::vector<user_info_t> Ivand_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Ivand", Ivand_prefix_default_order);
}

TEST(Easy, SearchUsersByNameWithDurations) {
  phone_book_t book;

  ASSERT_TRUE(book.create_user("123", "Ivan"));
  ASSERT_TRUE(book.create_user("12", "Ivan"));
  ASSERT_TRUE(book.create_user("1", "Iven"));
  ASSERT_TRUE(book.create_user("", "Ivbn"));
  ASSERT_TRUE(book.create_user("1234", "Ivan"));
  ASSERT_TRUE(book.create_user("12345", "Ivan"));
  ASSERT_TRUE(book.create_user("124", "Nikita"));
  ASSERT_TRUE(book.create_user("1244", "Nikita"));
  ASSERT_TRUE(book.create_user("2", "Bleg"));
  ASSERT_FALSE(book.create_user("2", "NotOleg"));

  ASSERT_TRUE(book.add_call({"2", 10}));
  ASSERT_TRUE(book.add_call({"1234", 5}));
  ASSERT_TRUE(book.add_call({"1244", 3}));
  ASSERT_TRUE(book.add_call({"", 1}));
  ASSERT_TRUE(book.add_call({"", 1}));
  ASSERT_TRUE(book.add_call({"1244", 2}));
  ASSERT_TRUE(book.add_call({"", 1}));

  std::vector<user_info_t> empty_prefix_default_order = {
      {{"2", "Bleg"}, 10},
      {{"1234", "Ivan"}, 5},
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
      {{"", "Ivbn"}, 3},
      {{"1", "Iven"}, 0},
      {{"1244", "Nikita"}, 5},
      {{"124", "Nikita"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("", empty_prefix_default_order);

  std::vector<user_info_t> I_prefix_default_order = {
      {{"1234", "Ivan"}, 5},
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
      {{"", "Ivbn"}, 3},
      {{"1", "Iven"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("I", I_prefix_default_order);

  std::vector<user_info_t> D_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("D", D_prefix_default_order);

  std::vector<user_info_t> Iv_prefix_default_order = {
      {{"1234", "Ivan"}, 5},
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
      {{"", "Ivbn"}, 3},
      {{"1", "Iven"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Iv", Iv_prefix_default_order);

  std::vector<user_info_t> Iva_prefix_default_order = {
      {{"1234", "Ivan"}, 5},
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Iva", Iva_prefix_default_order);

  std::vector<user_info_t> Ivan_prefix_default_order = {
      {{"1234", "Ivan"}, 5},
      {{"12", "Ivan"}, 0},
      {{"123", "Ivan"}, 0},
      {{"12345", "Ivan"}, 0},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Ivan", Ivan_prefix_default_order);

  std::vector<user_info_t> Ivand_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Ivand", Ivand_prefix_default_order);

  ASSERT_TRUE(book.add_call({"123", 7}));
  ASSERT_FALSE(book.add_call({"7", 8}));
  ASSERT_TRUE(book.add_call({"12", 6}));
  ASSERT_TRUE(book.add_call({"1", 3}));
  ASSERT_TRUE(book.add_call({"", 12}));
  ASSERT_TRUE(book.add_call({"1234", 20}));
  ASSERT_TRUE(book.add_call({"12345", 7}));
  ASSERT_TRUE(book.add_call({"124", 12}));
  ASSERT_TRUE(book.add_call({"1244", 22}));
  ASSERT_TRUE(book.add_call({"2", 17}));

  std::vector<user_info_t> changed_empty_prefix_default_order = {
      {{"2", "Bleg"}, 27},
      {{"1234", "Ivan"}, 25},
      {{"123", "Ivan"}, 7},
      {{"12345", "Ivan"}, 7},
      {{"12", "Ivan"}, 6},
      {{"", "Ivbn"}, 15},
      {{"1", "Iven"}, 3},
      {{"1244", "Nikita"}, 27},
      {{"124", "Nikita"}, 12},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("", changed_empty_prefix_default_order);

  std::vector<user_info_t> changed_I_prefix_default_order = {
      {{"1234", "Ivan"}, 25},
      {{"123", "Ivan"}, 7},
      {{"12345", "Ivan"}, 7},
      {{"12", "Ivan"}, 6},
      {{"", "Ivbn"}, 15},
      {{"1", "Iven"}, 3},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("I", changed_I_prefix_default_order);

  std::vector<user_info_t> changed_D_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("D", changed_D_prefix_default_order);

  std::vector<user_info_t> changed_Iv_prefix_default_order = {
      {{"1234", "Ivan"}, 25},
      {{"123", "Ivan"}, 7},
      {{"12345", "Ivan"}, 7},
      {{"12", "Ivan"}, 6},
      {{"", "Ivbn"}, 15},
      {{"1", "Iven"}, 3},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Iv", changed_Iv_prefix_default_order);

  std::vector<user_info_t> changed_Iva_prefix_default_order = {
      {{"1234", "Ivan"}, 25},
      {{"123", "Ivan"}, 7},
      {{"12345", "Ivan"}, 7},
      {{"12", "Ivan"}, 6},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Iva", changed_Iva_prefix_default_order);

  std::vector<user_info_t> changed_Ivan_prefix_default_order = {
      {{"1234", "Ivan"}, 25},
      {{"123", "Ivan"}, 7},
      {{"12345", "Ivan"}, 7},
      {{"12", "Ivan"}, 6},
  };
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Ivan", changed_Ivan_prefix_default_order);

  std::vector<user_info_t> changed_Ivand_prefix_default_order = {};
  CHECK_SEARCH_USERS_BY_NAME_BY_ORDER("Ivand", changed_Ivand_prefix_default_order);
}

#undef CHECK_SEARCH_USERS_BY_NAME_BY_ORDER

#undef CHECK_SEARCH_USERS_BY_ORDER
#undef CHECK_SEARCH_USERS_BY_ORDER_COUNT



TEST(Easy, ClearSizeEmpty) {
  phone_book_t book;

  ASSERT_EQ(book.size(), 0);
  ASSERT_TRUE(book.empty());
  ASSERT_EQ(book.get_calls(0, 100), std::vector<call_t>());
  ASSERT_EQ(book.search_users_by_number("", 100), std::vector<user_info_t>());
  ASSERT_EQ(book.search_users_by_name("", 100), std::vector<user_info_t>());

  ASSERT_TRUE(book.create_user("123", "Ivan"));
  ASSERT_TRUE(book.add_call({"123", 10}));
  ASSERT_EQ(book.size(), 1);
  ASSERT_FALSE(book.empty());
  ASSERT_EQ(book.get_calls(0, 100), std::vector<call_t>({{"123", 10}}));
  ASSERT_EQ(book.search_users_by_number("", 100), std::vector<user_info_t>({{{"123", "Ivan"}, 10}}));
  ASSERT_EQ(book.search_users_by_name("", 100), std::vector<user_info_t>({{{"123", "Ivan"}, 10}}));

  book.clear();
  ASSERT_EQ(book.size(), 0);
  ASSERT_TRUE(book.empty());
  ASSERT_EQ(book.get_calls(0, 100), std::vector<call_t>());
  ASSERT_EQ(book.search_users_by_number("", 100), std::vector<user_info_t>());
  ASSERT_EQ(book.search_users_by_name("", 100), std::vector<user_info_t>());
}
