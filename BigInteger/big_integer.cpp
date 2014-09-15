#include "big_integer.h"

#include <cmath>
#include <string>
#include <complex>

const int LOG = 32;
const uint64_t MASK = 0x00000000FFFFFFFF;
uint64_t power2[33] = {1,	2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536,
                                 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864,
                                 134217728, 268435456, 536870912, 1073741824, 2147483648UL, 4294967296UL};

//////////////////////////////////////////////////////////////////////
///////// Useful functions which isn't declared in class /////////////
//////////////////////////////////////////////////////////////////////

// returns number 0b['0'x32]11...10...00 with number of '1' equals size
uint64_t genLeftMask(int size) {
    assert(size <= 32);
    uint64_t result = 0;
    for (int i = LOG - 1; LOG - i - 1 < size; i--) {
        result |= power2[i];
    }

    return result;
}
// returns number 0b['0'x32]00...01...11 with number of '1' equals size
uint64_t genRightMask(int size) {
    assert(size <= 32);
    uint64_t result = 0;
    for (int i = 0; i < size; i++) {
        result |= power2[i];
    }

    return result;
}

// from binary mask to long value (mask and value are unsigned)
uint64_t genFromMask(std::vector <bool> mask) {
    assert(mask.size() <= 32);
    uint64_t result = 0;
    for (unsigned i = 0; i < mask.size(); i++) {
        result |= power2[i] * mask[i];
    }

    return result;
}

// abs(a) < abs(b)
bool absLess(big_integer const& a, big_integer const& b) {
    int i1, i2;
    for (i1 = (int) (a.data.size() - 1); i1 > 0 && a.data[i1] == 0; i1--);
    for (i2 = (int) (b.data.size() - 1); i2 > 0 && b.data[i2] == 0; i2--);

    if (i1 != i2)
        return i1 < i2;
    for (; i1 >= 0; i1--) {
        if (a.data[i1] != b.data[i1])
            return a.data[i1] < b.data[i1];
    }

    return false;
}

// set x and y to one size
void setSizeTwoSigned(big_integer *x, big_integer *y) {
    x->from2sCompliment();
    y->from2sCompliment();
    x->setSize((int) std::max(x->data.size(), y->data.size()));
    y->setSize((int) std::max(x->data.size(), y->data.size()));
    x->to2sCompliment();
    y->to2sCompliment();
}

// returns x / y and puts x % y into first argument
big_integer divMod(big_integer &x, big_integer &y) {
    x.from2sCompliment();
    y.from2sCompliment();

    big_integer result(0);
    result.twoCompilment = false;
    bool newSign = x.sign ^ y.sign, oldSign = x.sign;
    if (absLess(x, y)) {
        return big_integer(0);
    }
    else {
        x.sign = false;
        y.sign = false;
        x.setSize();
        y.setSize();

        y.from2sCompliment();
        // most significant word should be >= MASK / 2
        int msb = y.leadingBit((int) (y.data.size() - 1));
        y <<= LOG - msb - 2;
        x <<= LOG - msb - 2;
        y.setSize();
        x.setSize();

        long long m = (long long int) (x.data.size() - y.data.size()), n = (long long int) (y.data.size() - 1);
        big_integer tmp, tmp1;
        result.data.resize((size_t) m);
        uint64_t q;
        for (long long j = m - 1; j >= 0; j--) {
            q =  ((uint64_t) x.data[n + j + 1] << LOG) + x.data[n  + j];
            q /= y.data[n];
            q = std::min(q, power2[LOG] - 1);
            tmp = y;
            tmp.mulByWord(q, (int) j);
            while (absLess(x, tmp)) {
                tmp1.data.resize((size_t) (j + 1));
                tmp1.data[j] = 1;
                q--;
                tmp.unsignedSub(tmp1);
            }
            x.unsignedSub(tmp);
            result.data[j] = (uint32_t) q;
        }
    }

    x.setSize();
    if (x != 0)
        x.sign = oldSign;
    x.to2sCompliment();
    result.setSize();
    result.sign = newSign;
    result.twoCompilment = false;
    result.to2sCompliment();

    return result;
}

int abs_compare(big_integer const& a, big_integer const& b, int shift)
{
    if (a.data.size() > b.data.size() + shift) {
        return 1;
    }
    if (a.data.size() < b.data.size() + shift) {
        return -1;
    }
    for (int i = (int) (a.data.size() - 1); i >= shift; i--) {
        if (a.data[i] > b.data[i - shift]) {
            return 1;
        }
        if (a.data[i] < b.data[i - shift]) {
            return -1;
        }
    }
    for (int i = shift - 1; i >= 0; i--) {
        if (a.data[i] > 0) {
            return 1;
        }
    }
    return 0;
}

// return a % b in decimal
// set quotient to a / b;
uint64_t mod(big_integer a, uint64_t b, big_integer *quotient) {
    quotient->data.resize(a.data.size());
    uint64_t r = 0, l, tmp1, tmp2, x;
    int i;
    for (i = (int) (a.data.size() - 1); i >= 0; i--) {
        l = a.data[i];
        tmp1 = (l & 0xFFFF);
        tmp2 = (l >> 16) + (r << 16);
        x = tmp2 / b;
        r = tmp2 % b;
        quotient->data[i] = (uint32_t) (x << 16);
        tmp1 += (r << 16);
        x = tmp1 / b;
        r = tmp1 % b;
        quotient->data[i] += x;
    }

    for (i = (int) (quotient->data.size() - 1); i >= 0; i--) {
        if (quotient->data[i] != 0)
            break;
    }
    quotient->data.resize((size_t) (i + 1));

    return r;
}

//////////////////////////////////////////////////////////////////////
/////////////////////////// Class methods ////////////////////////////
//////////////////////////////////////////////////////////////////////

big_integer::big_integer() {
    twoCompilment = true;
    sign = false;
    data.push_back(0);
}

big_integer::big_integer(big_integer const& other) {
    twoCompilment = other.twoCompilment;
    sign = other.sign;
    data = other.data;
}

big_integer::big_integer(big_integer &&other) {
    twoCompilment = other.twoCompilment;
    sign = other.sign;
    data = other.data;
}

big_integer::big_integer(int a) {
    data.push_back((uint32_t) std::abs(a));
    if (a < 0) {
        twoCompilment = false;
        sign = true;
        to2sCompliment();
    }
    else
        sign = false;
    twoCompilment = true;
}

big_integer::big_integer(std::string const& str) {
    if (str.size() == 0)
        throw "Empty string";
    // for further explanations:
    //      std::basic_string<char, ::std::char_traits, ::std::allocator>::const_iterator
    for (auto it = str.begin(); it != str.end(); it++) {
        if (!(isdigit(*it) || (*it == '-' && it == str.begin())))
            throw "Incorrect string";
    }

    std::string s = str;
    sign = false;
    if (s[0] == '-') {
        sign = true;
        s = s.substr(1, s.size());
    }

    unsigned i;
    for (i = 0; i < str.size() - 1 && str[i] == '0'; i++);
    s = s.substr(i, s.size());

    unsigned size = (unsigned int) s.size();
    // ISO forbids static for some reason
    unsigned short *in = new unsigned short[size];
    for (i = 0; i < size; i++)
        in[i] = (unsigned short) (s[i] - '0');

    std::vector <bool> tmp;
    bool zero = false;
    for (i = 0; !zero; i++) {
        tmp.push_back((bool) (in[size - 1] & 1));
        zero = true;
        for (unsigned j = 0; j < size; j++) {
            if (in[j] % 2 && j < size - 1) {
                in[j + 1] += 10;
            }
            in[j] >>= 1;

            if (zero && in[j])
                zero = false;
        }

        if (tmp.size() == (unsigned) LOG) {
            data.push_back((uint32_t) genFromMask(tmp));
            tmp.clear();
        }
    }
    if (!tmp.empty())
        data.push_back((uint32_t) genFromMask(tmp));

    // -0 case
    if (data.size() == 0 && data.front() == 0)
        sign = false;

    free(in);

    twoCompilment = false;
    to2sCompliment();
}

big_integer::~big_integer() {
}

big_integer& big_integer::operator=(big_integer const& other) {
    twoCompilment = other.twoCompilment;
    sign = other.sign;
    data = other.data;
    return *this;
}

void big_integer::mulByWord(uint64_t val, int offset) {
    (*this) <<= offset * LOG;
    setSize();
    uint64_t carry = 0, tmp;
    for (unsigned i = 0; i < data.size(); i++) {
        tmp = data[i] * val + carry;
        carry = tmp >> LOG;
        data[i] = (uint32_t) tmp;
    }
    if (carry)
        data.push_back((uint32_t) carry);
}

void big_integer::unsignedAdd(big_integer const& rvalue) {
    uint64_t carry = 0, tmp;
    unsigned length = (unsigned) std::min(data.size(), rvalue.data.size());
    unsigned i;
    for (i = 0; i < length; i++) {
        tmp = (uint64_t)data[i] + (uint64_t)rvalue.data[i] + (uint64_t)carry;
        carry = tmp >> LOG;
        data[i] = (uint32_t) tmp;
    }

    for(; i < rvalue.data.size() && carry > 0; i++) {
        tmp = (uint64_t)rvalue.data[i] + (uint64_t)carry;
       carry = tmp >> LOG;
        data.push_back((uint32_t) tmp);
    }

    for (; i < data.size() && carry > 0; i++) {
        tmp = (uint64_t)data[i] + (uint64_t)carry;
        carry = tmp >> LOG;
        data[i] = (uint32_t) tmp;
    }

    if (carry) {
        data.push_back((uint32_t) carry);
    }
}

void big_integer::unsignedSub(big_integer const& rvalue) {
    big_integer a = *this, b = rvalue;
    a.sign = false;
    b.sign = false;
    if (a < b)
        std::swap(a, b);

    bool carry = false;
    unsigned i;
    uint64_t tmp;
    for (i = 0; i < b.data.size(); i++) {
        tmp = a.data[i];
        if (a.data[i] < b.data[i] + carry) {
            tmp += power2[LOG];
            carry = true;
        }
        else {
            carry = false;

        }
        a.data[i] = (uint32_t) (tmp - (b.data[i] + carry));
    }
    for (; i < a.data.size() && carry; i++) {
        tmp = a.data[i];
        if (a.data[i] < carry) {
            tmp += power2[LOG];
            carry = true;
        }
        else {
            carry = false;
        }
        a.data[i] = (uint32_t) (tmp - carry);
    }

    a.setSize();
    data = a.data;
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
    big_integer r = rhs;
    int size = (int) std::max(data.size(), r.data.size());
    setSize(size);
    r.setSize(size);
    if (sign) {
        data.push_back((uint32_t) MASK);
    }
    else {
        data.push_back(0);
    }
    r.setSize(size);
    if (r.sign) {
        r.data.push_back((uint32_t) MASK);
    }
    else {
        r.data.push_back(0);
    }

    unsignedAdd(r);
    data.resize((size_t) (size + 1));

    sign = (bool) (data.back() & power2[LOG - 1]);
    setSize();

    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
    big_integer result = (*this + (-rhs));
    this->data = result.data;
    this->sign = result.sign;
    this->twoCompilment = result.twoCompilment;
    return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
    from2sCompliment();
    big_integer b(rhs);
    b.from2sCompliment();

    std::vector <uint64_t> newData(data.size() + b.data.size());
    for (unsigned i = 0; i < newData.size(); i++) {
        newData[i] = 0;
    }
    for (unsigned i = 0; i < data.size(); i++) {
        for (unsigned j = 0; j < b.data.size(); j++) {
            newData[i + j] += (uint64_t) data[i] * b.data[j];
            if (i + j < newData.size())
                newData[i + j + 1] += ((newData[i + j]) >> LOG);
            newData[i + j] &= MASK;
        }
    }

    data.setUint64Vector(newData);
    setSize();
    sign ^= b.sign;
    b.from2sCompliment();

    return *this;
}

int big_integer::leadingBit(int n) {
    for (int i = LOG - 1; i >= 0; i--) {
        if (power2[i] & data[n])
            return i;
    }

    return -1;
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
    setSize();
    big_integer l = big_integer(*this), r = big_integer(rhs), tmp;
    from2sCompliment();
    sign = l.sign ^ r.sign;
    twoCompilment = false;
    l.from2sCompliment();
    r.from2sCompliment();
    l.sign = r.sign = false;
    r.setSize();
    l.setSize();

    if ((int)l.data.size() - (int)r.data.size() + 1 <= 0) {
        data.resize(1);
        data[0] = 0;
        sign = false;
        to2sCompliment();
        return *this;
    }
    else if (r.data.size() == 1) {
        from2sCompliment();
        uint32_t d = r.data.front();
        uint64_t carry = 0, curr;
        for (int i = (int) (data.size() - 1); i >= 0; i--) {
            curr = (carry << LOG) + data[i];
            data[i] = (uint32_t) (curr / d);
            carry = curr % d;
        }
        to2sCompliment();
        return *this;
    }

    if (r.data.back() < power2[LOG - 1]) {
        int msb = r.leadingBit((int) (r.data.size() - 1));
        l <<= LOG - msb - 1;
        r <<= LOG - msb - 1;

        r.setSize();
        l.setSize();
    }

    data.resize(l.data.size() - r.data.size() + 1);
    int result = 0;
    for (int i = (int) (l.data.size() - r.data.size()); i >= 0; i--) {
        int vsize = (int) l.data.size(), rsize = (int) r.data.size();
        uint32_t u1 = l.data[vsize - 1],
                u2 = vsize - 2 >= 0 ? l.data[vsize - 2] : 0,
                u3 = (vsize - 3 >= 0) ? l.data[vsize - 3] : 0,
                v1 = r.data[rsize - 1],
                v2 = (rsize - 2 >= 0) ? r.data[rsize - 2] : 0;
        uint64_t u = u1 > v1 ? u1 : ((uint64_t)u1 << LOG) | u2;
        uint64_t stq = std::min(u / v1, MASK), str = u % v1;
        int it = 0;
        do {
            if (stq == power2[LOG] || stq * v2 > (str << LOG) + u3) {
                stq--;
                str += v1;
            }
        } while(str < power2[LOG] && ++it < 2);
        tmp = r;
        tmp.mulByWord(stq, 0);
        result = abs_compare(l, tmp, i);
        if (result >= 0) {
            tmp <<= i * LOG;
            l -= tmp;
            data[i] = (uint32_t) stq;
            if (result == 0) {
                break;
            }
        } else {
            data[i] = 0;
        }
    }

    setSize();
    to2sCompliment();
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
    return *this -= *this / rhs * rhs;
}

big_integer& big_integer::operator&=(big_integer const& rhs) {
    big_integer x = *this, y = rhs;
    setSizeTwoSigned(&x, &y);

    for (unsigned i = 0; i < x.data.size(); i++) {
        x.data[i] &= y.data[i];
    }

    data = x.data;
    sign = (bool) (x.data[x.data.size() - 1] & power2[LOG - 1]);
    twoCompilment = x.twoCompilment;

    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs)
{
    big_integer x = *this, y = rhs;
    setSizeTwoSigned(&x, &y);

    for (unsigned i = 0; i < x.data.size(); i++) {
        x.data[i] |= y.data[i];
    }

    data = x.data;
    sign = (bool) (x.data[x.data.size() - 1] & power2[LOG - 1]);
    twoCompilment = x.twoCompilment;

    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
    big_integer x = *this, y = rhs;
    setSizeTwoSigned(&x, &y);

    for (unsigned i = 0; i < x.data.size(); i++) {
        x.data[i] ^= y.data[i];
    }

    data = x.data;
    sign = (bool) (x.data[x.data.size() - 1] & power2[LOG - 1]);
    twoCompilment = x.twoCompilment;

    return *this;
}

big_integer& big_integer::operator<<=(int rhs) {
    from2sCompliment();

    // shift elements of data (if b > 32)
    int step = rhs / LOG;
    // + 1 for possible carry
    data.resize(data.size() + step + 1);
    if (step) {
        for (int i = (int) (data.size() - step - 1); i >= 0; i--) {
            data[step + i] = data[i];
        }
        for (int i = 0; i < step; i++) {
            data[i] = 0;
        }
    }

    // shift every element and carry to next
    int shift = rhs % LOG;
    if (shift) {
        uint64_t carry = 0, tmp, mask = genLeftMask(shift);
        for (unsigned i = 0; i < data.size() - 1; i++) {
            tmp = data[i] & mask;
            tmp >>= LOG - shift;
            data[i] <<= shift;
            data[i] &= MASK;
            data[i] |= carry;
            carry = tmp;
        }
        data.back() += carry;
        // segfault somehow
//        if (carry) {
//            data.push_back(carry);
//        }
    }

    to2sCompliment();
    setSize();
    return *this;
}

big_integer& big_integer::operator>>=(int rhs) {
    // shift elements of data (if b > 32)
    int step = rhs / LOG;
    if (step) {
        for (unsigned i = 0; i < data.size() - step; i++) {
            data[i] = data[i + step];
        }
        setSize();
    }

    // shift every element and carry to next
    int shift = rhs % LOG;
    if (shift) {
        uint64_t carry = 0, tmp, mask = genRightMask(shift),
                lMask = genLeftMask(shift);
        for (int i = (int) (data.size() - 1); i >= 0; i--) {
            tmp = data[i] & mask;
            tmp <<= LOG - shift;
            data[i] >>= shift;
            if (sign && (unsigned) i == data.size() - 1)
                data[i] |= lMask;
            data[i] &= MASK;
            data[i] |= carry;
            carry = tmp;
        }
    }

    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer tmp = *this;
    if (tmp != big_integer(0)) {
        tmp.from2sCompliment();
        tmp.sign = !tmp.sign;
        tmp.to2sCompliment();
    }
    return tmp;
}

big_integer big_integer::operator~() const {
    return -(*this) - 1;
}

big_integer& big_integer::operator++() {
    operator+=(1);
    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer& big_integer::operator--() {
    operator-=(1);
    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer r = *this;
    --*this;
    return r;
}

big_integer operator+(big_integer a, big_integer const& b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b){
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const& b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const& b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const& b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const& b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

bool operator==(big_integer const& a, big_integer const& b) {
    if (a.sign != b.sign)
        return false;

    big_integer x = a, y = b;
    x.to2sCompliment();
    y.to2sCompliment();
    x.setSize();
    y.setSize();
    if (x.data.size() != y.data.size())
        return false;
    for (int i = (int) (x.data.size() - 1); i >= 0; i--) {
        if (x.data[i] != y.data[i])
            return false;
    }
    return true;
}

bool operator!=(big_integer const& a, big_integer const& b) {
    return !(a == b);
}

bool operator<(big_integer const& a, big_integer const& b) {
    if (a.sign != b.sign)
        return a.sign;

    return absLess(a, b);
}

bool operator>(big_integer const& a, big_integer const& b) {
    return !(a < b);
}

bool operator<=(big_integer const& a, big_integer const& b) {
    return ((a < b) || (a == b));
}

bool operator>=(big_integer const& a, big_integer const& b) {
    return ((a > b) || (a == b));
}

void big_integer::setSize() {
    int i;
    if (sign) {
        for (i = (int) (data.size() - 1); i > 0 && data[i] == MASK; i--);
    }
    else {
        for (i = (int) (data.size() - 1); i > 0 && data[i] == 0; i--);
    }
    data.resize((size_t) (i + 1));
}

void big_integer::setSize(int dest) {
    int val;
    if (!sign)
        val = 0;
    else
        val = (int) MASK;

    if (data.size() < (unsigned) dest) {
        int i = (int) data.size();
        data.resize((size_t) dest);
        for (; (unsigned) i < data.size(); i++)
            data[i] = (uint32_t) val;
    }
}

void big_integer::unsignedNot() {
    uint64_t mask = genRightMask(LOG);
    for (unsigned i = 0; i < data.size(); i++)
        data[i] = (uint32_t) ((-data[i] - 1) & mask);
}

void big_integer::to2sCompliment() {
    if (!twoCompilment) {
        // -0 case
        if (data.size() == 1 && data.front() == 0)
            sign = false;
        if (sign) {
            // if lead bit used
            if (data.back() & power2[LOG - 1]) {
                data.resize(data.size() + 1);
            }
            unsignedNot();
            unsignedAdd(big_integer(1));

            int lb = leadingBit((int) (data.size() - 1));
            for (lb++; lb < LOG; lb++) {
                data.back() += power2[lb];
            }
        }
        // lead bit must be zero, need for addition
        else {
            if (data.back() & power2[LOG]) {
                data.resize(data.size() + 1);
                data.back() = 0;
            }
        }
        twoCompilment = true;
    }
}

void big_integer::from2sCompliment() {
    if (twoCompilment) {
        // -0 case
        if (data.size() == 1 && data.front() == 0)
            sign = false;
        if (sign) {
            unsignedNot();
            setSize();
            unsignedAdd(big_integer(1));
        }
        else {
            // lead integer might be zero
            setSize();
        }
        twoCompilment = false;
    }
}

std::string intToString(int x, int digits) {
    std::string result = "";
    for (int i = 0; i < digits; i++) {
        result = (char)(x % 10 + (int)'0') + result;
        x /= 10;
    }
    return result;
}

std::string to_string(big_integer const& a)
{
    big_integer curr(a);
    std::string result;
    big_integer quotient(1);
    bool neg = curr.sign;
    if (neg)
        curr.from2sCompliment();
    while (quotient.data.size() > 1 ||
           (quotient.data.size() == 1 && quotient.data.front() != 0)) {
        result = intToString((int) mod(curr, 1000, &quotient), 3) + result;
        curr = quotient;
    }

    // remove leading zeros
    unsigned i;
    for (i = 0; i < 2 && i < result.size() && result[i] == '0'; i++);
    result = result.substr(i, result.size());

    if (neg)
        result = "-" + result;
    return result;
}

big_integer::operator std::string () {
    return to_string(*this);
}

std::ostream& operator<<(std::ostream& s, big_integer const& a) {
    return s << to_string(a);
}

std::istream& operator>>(std::istream& s, big_integer &a) {
    std::string tmp;
    s >> tmp;
    a = big_integer(tmp);
    return s;
}
