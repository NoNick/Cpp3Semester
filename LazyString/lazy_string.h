#ifndef LAZY_STRING_H
#define LAZY_STRING_H

#include <string>

class lazy_string
{
public:
    class proxy {
    private:
        friend class lazy_string;
        lazy_string *s;
        int pos;

        proxy(lazy_string *s, int pos) : s(s), pos(pos) {}
        proxy(proxy const&) = default;
        proxy(proxy &&) = default;
    public:
        operator char() { return s->getPos(pos); }
        proxy operator=(char c) {
            s->set(pos, c);
            return *this;
        }
    };

    lazy_string();
    lazy_string(lazy_string const&);
    lazy_string(lazy_string &&);
    lazy_string(std::string const&);
    ~lazy_string();
    std::string str() const;
    proxy operator[](int);
    char operator[](int) const;
    lazy_string operator+(lazy_string const&) const;
    lazy_string operator+=(lazy_string const&);

private:
    friend class proxy;
    char getPos(int) const;
    void set(int, char);

    void copy();
    std::string *data;
    int *linksCnt;
};

#endif // LAZY_STRING_H
