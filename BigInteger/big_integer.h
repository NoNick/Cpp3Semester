#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iosfwd>
#include <vector>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include "container.cpp"

class big_integer
{
public:
    bool sign, twoCompilment;

    big_integer();
    // copy constructor
    big_integer(big_integer const& other);
    // move constructor
    big_integer(big_integer&& other);
    big_integer(int a);
    explicit big_integer(std::string const& str);
    ~big_integer();

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);

    friend std::string to_string(big_integer const& a);
    operator std::string ();
    // abs(a) < abs(b)
    static bool absLess(big_integer const& a, big_integer const& b);

private:
    // copy-on-write & small object implementations
    container data;

    // remove leading zeros (if pos) && maxints (if neg)
    void setSize();
    // set data size to dest
    void setSize(int);

    // converting to two's complement representation
    void to2sCompliment();
    // converting from two's complement representation
    void from2sCompliment();

    void unsignedNot();
    void unsignedAdd(big_integer const&);
    void unsignedSub(big_integer const&);
    // (*this) * (val << (LOG * offset))
    void mulByWord(uint64_t val, int offset);

    // returns value from 0 to LOG - 1: first true bit in data[n]
    // returns -1 if all bits are false
    int leadingBit(int n);

    // set x and y to one size
    static void setSizeTwoSigned(big_integer *x, big_integer *y);
    // returns x / y and puts x % y into first argument
    static big_integer divMod(big_integer &x, big_integer &y);
    // return a % b in decimal
    // set quotient to a / b;
    static uint64_t mod(big_integer a, uint64_t b, big_integer *quotient);
    static int abs_compare(big_integer const& a, big_integer const& b, int shift);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);
std::istream& operator>>(std::istream& s, big_integer &a);

#endif // BIG_INTEGER_H
