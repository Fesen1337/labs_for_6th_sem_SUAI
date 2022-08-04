#include "number.h"

using namespace std;

number_t::number_t() : x(0) {}


number_t::number_t(int64_t o_x) : x(o_x){}

number_t::number_t(double o_x) : x(o_x) {}


number_t::number_t(int o_x) : x(o_x) {}

number_t::number_t(const number_t &o_number_t) : x(o_number_t.x) {}

number_t::operator int64_t() const
{
    return x;
}

number_t::operator int() const
{
    return int(x);
}

number_t::operator float() const
{
    return float(x);
}

number_t::operator bool() const
{
    return bool(x);
}


number_t::operator std::string() const
{
    return to_string(int64_t(x));
}

number_t& number_t::operator=(const number_t &o_x) = default;
number_t& number_t::operator=(const int64_t &o_x) {
    x = o_x;
    return *this;
}
/*
number_t& number_t::operator+(const number_t &val){
    this->x = this->x + val.x;
    return *this;
}

number_t& number_t::operator+(const int &val) {
    this->x += val;
    return *this;
}*/

number_t operator+(const int64_t &val, number_t &val2){
    number_t res;
    res.x = val + val2.x;
    return res;
}


number_t operator+(number_t &val2, const int64_t &val){
    number_t res;
    res.x = val + val2.x;
    return res;
}

/*number_t& number_t::operator-(const number_t &val){
  this->x = this->x + val.x;
  return *this;
}*/

bool number_t::operator==(const number_t &val) const {
    return this->x == val.x;
}
bool number_t::operator==(const int64_t &val) const {
    return this->x == static_cast <long long>(val);
}
bool number_t::operator!=(const number_t &val) const {
    return this->x != val.x;
}
bool number_t::operator!=(const int64_t &val) const {
    return this->x != static_cast <long long>(val);
}
bool number_t::operator>(const number_t &val) const {
    return this->x > val.x;
}
bool number_t::operator<(const number_t &val) const {
    return this->x < val.x;
}
bool number_t::operator>=(const number_t &val) const {
    return this->x >= val.x;
}
bool number_t::operator<=(const number_t &val) const {
    return this->x <= val.x;
}
bool number_t::operator<=(const int64_t &val) const {
    return this->x <= static_cast <long long>(val);
}



std::ostream &operator<<(std::ostream &os, const number_t& number) {
    return (os << number.x);
}

std::istream &operator>>(std::istream &is, number_t& number) {
    return is >> number.x;
}



bool operator==(const int64_t &val, const number_t &val2) {
    return static_cast <long long>(val) == val2.x;
}
bool operator!=(const int64_t &val, const number_t &val2) {
    return static_cast <long long>(val) != val2.x;
}
bool operator<(const int64_t &val, const number_t &val2) {
    return static_cast <long long>(val) < val2.x;
}
bool number_t::operator<(const int64_t &val) const {
    return this->x < static_cast <long long>(val);
}
bool number_t::operator>(const int64_t &val) const {
    return this->x > static_cast <long long>(val);
}
bool operator>(const int64_t &val, const number_t &val2) {
    return static_cast <long long>(val) > val2.x;
}
bool operator<=(const int64_t &val, const number_t &val2) {
    return static_cast <long long>(val) <= val2.x;
}
bool operator>=(const int64_t &val, const number_t &val2) {
    return static_cast <long long>(val) >= val2.x;
}
bool number_t::operator>=(const int64_t &val) const {
    return this->x >= static_cast <long long>(val);
}

number_t& number_t::operator++(){
    x++;
    return *this;
}

number_t& number_t::operator--(){
    x--;
    return *this;
}

number_t number_t::operator++(int) {
    number_t temp{*this};
    ++(*this);
    return temp;
}

number_t number_t::operator--(int) {
    int64_t t = this->x;
    number_t temp(t);
    --(*this);
    return temp;
}

number_t &number_t::operator+=(const int64_t &val2) {
    x += val2;
    return *this;
}

number_t operator+(const number_t &left, const number_t &right) {
    number_t res;
    res.x = left.x + right.x;
    return res;
}

number_t operator-(const number_t &val, const int64_t &val2) {
    number_t res;
    res.x = val.x - val2;
    return res;
}

number_t operator-(const int64_t &val, const number_t &val2) {
    number_t res;
    res.x = val - val2.x;
    return res;
}

number_t number_t::operator-() const {
    number_t temp;
    temp.x = -this->x;
    return temp;
}

number_t number_t::operator+() const {
    return *this;
}

number_t &number_t::operator+=(const number_t &val2) {
    x += val2.x;
    return *this;
}

number_t& number_t::operator-=(const int64_t &val2){
    x -= val2;
    return *this;
}

number_t &number_t::operator-=(const number_t &val2) {
    x -= val2.x;
    return *this;
}

number_t operator-(const number_t &left, const number_t &right) {
    number_t res;
    res.x = left.x - right.x;
    return res;
}

number_t operator*(const number_t &left, const int64_t &right) {
    number_t res;
    res.x = left.x * right;
    return res;
}

number_t operator*(const int64_t &left, const number_t &right) {
    number_t res;
    res.x = left * right.x;
    return res;
}

number_t operator*(const number_t &left, const number_t &right) {
    number_t res;
    res.x = left.x * right.x;
    return res;
}

number_t number_t::operator*=(const int64_t &right) {
    this->x *= right;
    return *this;
}

number_t number_t::operator*=(const number_t &right) {
    this->x *= right.x;
    return *this;
}

number_t operator/(const number_t &left, const int64_t &right) {
    number_t res;
    res.x = left.x / right;
    return res;
}

number_t operator/(const int64_t &left, const number_t &right) {
    number_t res;
    res.x = left / right.x;
    return res;
}

number_t operator/(const number_t &left, const number_t &right) {
    number_t res;
    res.x = left.x / right.x;
    return res;
}

number_t number_t::operator/=(const int64_t &right) {
    this->x /= right;
    return *this;
}

number_t number_t::operator/=(const number_t &right) {
    this->x /= right.x;
    return *this;
}

number_t operator%(number_t &left, const int64_t &right) {
    return {int64_t(left.x) % right};
}

number_t operator%(const int64_t &left, number_t &right) {
    return {left % int64_t(right.x)};
}

number_t operator%(const number_t &left, const number_t &right) {
    return {int64_t(left.x) % int64_t(right.x)};
}

number_t number_t::operator%=(const int64_t &right) {
    this->x = int64_t(this->x) % right;
    return *this;
}

number_t number_t::operator%=(const number_t &right) {
    this->x = int64_t(this->x) % int64_t(right.x);
    return *this;
}

number_t operator^(number_t &left, const int64_t &right) {
    return {int64_t(left.x) ^ right};
}

number_t operator^(const int64_t &left, number_t &right) {
    return {left ^ int64_t(right.x)};
}

number_t operator^(const number_t &left, const number_t &right) {
    return {int64_t(left.x) ^ int64_t(right.x)};
}

number_t number_t::operator^=(const int64_t &right) {
    this->x = int64_t(this->x) ^right;
    return *this;
}

number_t number_t::operator^=(const number_t &right) {
    this->x = int64_t(this->x) ^int64_t(right.x);
    return *this;
}

number_t operator&(number_t &left, const int64_t &right) {
    return {int64_t(left.x) & right};
}

number_t operator&(const int64_t &left, number_t &right) {
    return {left & int64_t(right.x)};
}

number_t operator&(const number_t &left, const number_t &right) {
    return {int64_t(left.x) & int64_t(right.x)};
}

number_t number_t::operator&=(const int64_t &right) {
    this->x = int64_t(this->x) & right;
    return *this;
}

number_t number_t::operator&=(const number_t &right) {
    this->x = int64_t(this->x) & int64_t(right.x);
    return *this;
}

number_t operator|(number_t &left, const int64_t &right) {
    return {int64_t(left.x) | right};
}

number_t operator|(const int64_t &left, number_t &right) {
    return {left | int64_t(right.x)};
}

number_t operator|(const number_t &left, const number_t &right) {
    return {int64_t(left.x) | int64_t(right.x)};
}

number_t number_t::operator|=(const int64_t &right) {
    this->x = int64_t(this->x) | right;
    return *this;
}

number_t number_t::operator|=(const number_t &right) {
    this->x = int64_t(this->x) | int64_t(right.x);
    return *this;
}

number_t number_t::operator~() const {
    return {~(int64_t(this->x))};
}

number_t &number_t::operator<<=(const int64_t &right) {
    x <<= right;
    return *this;
}

number_t &number_t::operator>>=(const int64_t &right) {
    x >>= right;
    return *this;
}

number_t number_t::operator<<(const int64_t &right) const {
    return {int64_t(this->x) << right};
}

number_t number_t::operator>>(const int64_t &right) const {
    return {int64_t(this->x) >> right};
}