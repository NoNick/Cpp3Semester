#include <bits/stringfwd.h>

class parse_exception: public std::exception {
public:
    explicit parse_exception(const char* message): msg(message) {};
    explicit parse_exception(const std::string& message): msg(message) {};
    virtual ~parse_exception() throw(){}
    virtual const char* what() const throw() {
        return msg.c_str();
    }

protected:
    std::string msg;
};