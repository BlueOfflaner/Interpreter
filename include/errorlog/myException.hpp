#ifndef _MYEXCEPTION_HPP
#define _MYEXCEPTION_HPP

#include <string>

class MyException
{
public:
    MyException(std::string str);
    std::string what() const;
protected:
    std::string message;
};

#endif