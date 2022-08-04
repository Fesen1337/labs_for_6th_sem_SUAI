#include "gtest/gtest.h"

#include <limits>
#include <sstream>

#include "number.h"

TEST(Number, ConstructorsAndAssignments) {
  number_t a;
  ASSERT_EQ(a, number_t(0));

  number_t b(10);
  ASSERT_EQ(b, number_t(10));

  number_t c = b;
  ASSERT_EQ(c, b);
  ASSERT_EQ(c, 10);

  c = a;
  ASSERT_NE(c, b);
  ASSERT_NE(c, 10);
  ASSERT_EQ(c, a);
  ASSERT_EQ(c, 0);

  a = b = c = 10;
  ASSERT_EQ(a, 10);
  ASSERT_EQ(b, 10);
  ASSERT_EQ(c, 10);
}

TEST(Number, Casts) {
  number_t zero;
  number_t positive(1);
  number_t negative(-1);

  ASSERT_TRUE(static_cast<bool>(positive));
  ASSERT_TRUE(static_cast<bool>(negative));
  ASSERT_FALSE(static_cast<bool>(zero));

  ASSERT_EQ(0, static_cast<int64_t>(zero));
  ASSERT_EQ(1, static_cast<int64_t>(positive));
  ASSERT_EQ(-1, static_cast<int64_t>(negative));

  int64_t int_zero = static_cast<int64_t>(zero);
  int64_t int_positive = static_cast<int64_t>(positive);
  int64_t int_negative = static_cast<int64_t>(negative);
  ASSERT_EQ(0, int_zero);
  ASSERT_EQ(1, int_positive);
  ASSERT_EQ(-1, int_negative);

  number_t max_val(std::numeric_limits<int64_t>::max());
  ASSERT_EQ(std::numeric_limits<int64_t>::max(), static_cast<int64_t>(max_val));
  number_t min_val(std::numeric_limits<int64_t>::min());
  ASSERT_EQ(std::numeric_limits<int64_t>::min(), min_val);

  ASSERT_EQ("0", static_cast<std::string>(zero));
  ASSERT_EQ("1", static_cast<std::string>(positive));
  ASSERT_EQ("-1", static_cast<std::string>(negative));
}

TEST(Number, Comparisions) {
  number_t a = 10;
  number_t b = 10;
  number_t c = -1;
  number_t d = 100;

  ASSERT_TRUE(a == b && a == 10 && 10 == a);
  ASSERT_FALSE(a == c || a == d || b == c || b == d || a == 11 || 11 == a);

  ASSERT_TRUE(a != c && a != d && b != c && b != d && a != 11 && 11 != a);
  ASSERT_FALSE(a != b || a != 10 || 10 != a);

  ASSERT_TRUE(a < d && b < d && c < a && c < b && c < d && 10 < d && c < 10);
  ASSERT_FALSE(a < a || a < b || b < c || d < a || a < 10 || 10 < a);

  ASSERT_TRUE(d > a && d > b && a > c && b > c && d > c && d > 10 && 10 > c);
  ASSERT_FALSE(a > a || b > a || c > b || a > d || 10 > a || a > 10);

  ASSERT_TRUE(a <= b && b <= a && a <= 10 && 10 <= a && a <= 12 && 5 <= a &&
              a <= d && c <= 0);
  ASSERT_FALSE(a <= 2 || a <= -100 || d <= c);

  ASSERT_TRUE(b >= a && a >= b && 10 >= a && a >= 10 && 12 >= a && a >= 5 &&
              d >= a && 0 >= c);
  ASSERT_FALSE(2 >= a || -100 >= a || c >= d);
}

TEST(Number, InputOutput) {
  number_t a(1);
  number_t b(2);
  std::stringstream stream;

  stream << a << " " << b;
  ASSERT_EQ(stream.str(), "1 2");
  number_t c;
  number_t d;
  stream >> c >> d;
  ASSERT_EQ(a, c);
  ASSERT_EQ(b, d);
}

#define CHECK_BINARY_OP_INTERNAL(op, mop, name)                                \
  TEST(Number, Operation##name) {                                              \
    number_t a = 1024;                                                         \
    int64_t ia = 1024;                                                         \
    a = a op 2;                                                                \
    ia = ia op 2;                                                              \
    ASSERT_EQ(a, ia);                                                          \
                                                                               \
    a = 123;                                                                   \
    ia = 123;                                                                  \
                                                                               \
    a = 3 op a;                                                                \
    ia = 3 op ia;                                                              \
    ASSERT_EQ(a, ia);                                                          \
                                                                               \
    a = 321;                                                                   \
    ia = 321;                                                                  \
                                                                               \
    number_t b = a op a;                                                       \
    int64_t ib = ia op ia;                                                     \
    ASSERT_EQ(b, ib);                                                          \
                                                                               \
    b mop a mop 4;                                                             \
    ib mop ia mop 4;                                                           \
    ASSERT_EQ(a, ia);                                                          \
    ASSERT_EQ(b, ib);                                                          \
                                                                               \
    const number_t ca = 5;                                                     \
    const int64_t ica = 5;                                                     \
    a = ca op ca;                                                              \
    ia = ica op ica;                                                           \
    ASSERT_EQ(a, ia);                                                          \
                                                                               \
    a mop ca;                                                                  \
    ia mop ica;                                                                \
    ASSERT_EQ(a, ia);                                                          \
  }

#define CHECK_BINARY_OP(op, name) CHECK_BINARY_OP_INTERNAL(op, op## =, name)

CHECK_BINARY_OP(+, BinaryPlus)
CHECK_BINARY_OP(-, BinaryMinus)
CHECK_BINARY_OP(*, Multiplication)
CHECK_BINARY_OP(/, Division)
CHECK_BINARY_OP(%, Modulo)

CHECK_BINARY_OP(^, BitwiseXor)
CHECK_BINARY_OP(&, BitwiseAns)
CHECK_BINARY_OP(|, BitwiseOr)

#undef CHECK_OP
#undef CHECK_OP_INTERNAL

TEST(Number, Increment) {
  number_t a = 10;
  int64_t ia = 10;

  ++++++a;
  ++++++ia;
  ASSERT_EQ(a, ia);

  number_t b = a++;
  int64_t ib = ia++;
  ASSERT_EQ(a, ia);
  ASSERT_EQ(b, ib);
}
TEST(Number, Decrement) {
  number_t a = 10;
  int64_t ia = 10;

  ------a;
  ------ia;
  ASSERT_EQ(a, ia);

  number_t b = a--;
  int64_t ib = ia--;
  ASSERT_EQ(a, ia);
  ASSERT_EQ(b, ib);
}

TEST(Number, UnaryOperators) {
  const number_t a = 10;
  const int64_t ia = 10;

  ASSERT_EQ(-a, -ia);
  ASSERT_EQ(+a, +ia);
  ASSERT_EQ(!a, !ia);
  ASSERT_EQ(~a, ~ia);
}

TEST(Number, BinaryShifts) {
  number_t a = 10345;
  int64_t ia = 10345;

  ASSERT_EQ(static_cast<const number_t>(a) << 3, static_cast<const int64_t>(ia)
                                                     << 3);
  ASSERT_EQ(static_cast<const number_t>(a) >> 2,
            static_cast<const int64_t>(ia) >> 2);

  (a <<= 3) += 10;
  (ia <<= 3) += 10;
  ASSERT_EQ(a, ia);

  (a >>= 2) -= 12;
  (ia >>= 2) -= 12;
  ASSERT_EQ(a, ia);
}
