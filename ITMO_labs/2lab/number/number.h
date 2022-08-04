#pragma once

#include <cstdint>
#include <string>
#include <iostream>

class number_t {
private:
    int64_t x;

public:
    number_t();
    number_t(int64_t o_x);
    number_t(double o_x);
    number_t(int o_x);
    number_t(const number_t &o_number_t);
    //void set(int o_x);


    //Касты
    explicit operator int() const;
    explicit operator float() const;
    explicit operator bool() const;
    explicit operator int64_t() const;
    explicit operator std::string() const;

    // оператор присвоения
    number_t& operator=(const number_t &o_x);
    number_t& operator=(const int64_t &o_x);
    //бинарное сложение
    //number_t& operator+(const number_t &val);
    //number_t& operator+(const int &val);
    friend number_t operator+(number_t &val, const int64_t &val2);
    friend number_t operator+(const int64_t &val, number_t &val2);
    friend number_t operator+(const number_t& left, const number_t& right);

    friend number_t operator-(const number_t &val, const int64_t &val2);
    friend number_t operator-(const int64_t &val, const number_t &val2);
    friend number_t operator-(const number_t& left, const number_t& right);

    number_t & operator+=(const int64_t &val2);
    number_t& operator+=(const number_t &val2);

    number_t& operator-=(const int64_t &val2);
    number_t& operator-=(const number_t &val2);

    number_t operator-() const;
    number_t operator+() const;



    number_t& operator++();
    number_t& operator--();
    number_t operator++(int);
    number_t operator--(int);


    friend number_t operator*(const number_t &left, const int64_t & right);
    friend number_t operator*(const int64_t &left, const number_t & right);
    friend number_t operator*(const number_t& left, const number_t& right);

    number_t operator*=(const int64_t & right);
    number_t operator*=(const number_t& right);



    friend number_t operator/(const number_t &left, const int64_t & right);
    friend number_t operator/(const int64_t &left, const number_t & right);
    friend number_t operator/(const number_t& left, const number_t& right);

    number_t operator/=(const int64_t & right);
    number_t operator/=(const number_t& right);


    friend number_t operator%(number_t &left, const int64_t & right);
    friend number_t operator%(const int64_t &left, number_t & right);
    friend number_t operator%(const number_t& left, const number_t& right);

    number_t operator%=(const int64_t & right);
    number_t operator%=(const number_t& right);


    friend number_t operator^(number_t &left, const int64_t & right);
    friend number_t operator^(const int64_t &left, number_t & right);
    friend number_t operator^(const number_t& left, const number_t& right);

    number_t operator^=(const int64_t & right);
    number_t operator^=(const number_t& right);


    friend number_t operator&(number_t &left, const int64_t & right);
    friend number_t operator&(const int64_t &left, number_t & right);
    friend number_t operator&(const number_t& left, const number_t& right);

    number_t operator&=(const int64_t & right);
    number_t operator&=(const number_t& right);


    friend number_t operator|(number_t &left, const int64_t & right);
    friend number_t operator|(const int64_t &left, number_t & right);
    friend number_t operator|(const number_t& left, const number_t& right);

    number_t operator|=(const int64_t & right);
    number_t operator|=(const number_t& right);


    number_t operator~() const ;


    number_t operator<<(const int64_t &right) const;

    number_t operator>>(const int64_t &right) const;



    number_t &operator <<=(const int64_t & right);

    number_t &operator >>=(const int64_t & right);

    bool operator ==(const number_t&val) const;
    friend bool operator ==(const int64_t &val, const number_t&val2);
    bool operator ==(const int64_t&val) const;
    bool operator !=(const number_t& val) const;
    bool operator !=(const int64_t& val) const;
    friend bool operator !=(const int64_t&val, const number_t&val2);
    bool operator >(const number_t& val) const;
    bool operator >(const int64_t& val) const;
    friend bool operator >(const int64_t&val, const number_t&val2);
    bool operator <(const number_t& val) const;
    bool operator <(const int64_t& val) const;
    friend bool operator <(const int64_t& val, const number_t&val2);
    bool operator >=(const number_t& val) const;
    bool operator >=(const int64_t & val) const;
    friend bool operator >=(const int64_t & val, const number_t& val2);
    bool operator <=(const number_t& val) const;
    friend bool operator <=(const int64_t& val, const number_t& val2);
    bool operator <=(const int64_t & val) const;
    //оператор потокового ввода вывода
    friend std::ostream& operator<<(std::ostream& os, const number_t& number);
    friend std::istream& operator>>(std::istream& is, number_t& number);

    //инкременты декременты

};