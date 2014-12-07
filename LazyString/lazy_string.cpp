#include <string>

#include "lazy_string.h"

lazy_string::lazy_string() {
    data = new std::string();
    linksCnt = new int(1);
}

lazy_string::lazy_string(lazy_string const& other) {
    data = other.data;
    linksCnt = other.linksCnt;
    (*linksCnt)++;
    getPos(0);
}

lazy_string::lazy_string(lazy_string && other) {
    data = other.data;
    linksCnt = other.linksCnt;
    (*linksCnt)++;
    getPos(0);
}

lazy_string::lazy_string(std::string const& str) {
    data = new std::string(str);
    linksCnt = new int(1);
}

lazy_string::~lazy_string() {
    if (! --(*linksCnt)) {
        delete data;
        delete linksCnt;
    }
}

std::string lazy_string::str() const {
    return *data;
}

lazy_string::proxy lazy_string::operator[](int i) {
    return proxy(this, i);
}

char lazy_string::operator[](int i) const {
    return getPos(i);
}

lazy_string lazy_string::operator+(lazy_string const& a) const {
    return (*data + *a.data);
}

lazy_string lazy_string::operator+=(lazy_string const& a) {
    if (*linksCnt > 1)
        copy();
    data->operator +=(*a.data);
    return *this;
}

char lazy_string::getPos(int i) const {
    return (*data)[i];
}

void lazy_string::set(int pos, char c) {
    if (*linksCnt > 1)
        copy();
    char& c1 = data->operator[](pos);
    c1 = c;
}

void lazy_string::copy() {
    int & old = *linksCnt;
    linksCnt = new int(*linksCnt);
    data = new std::string(*data);
    --old;
}
