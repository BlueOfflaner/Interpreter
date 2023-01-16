#include "errorlog/myException.hpp"

MyException::MyException(std::string str) : message{str}
{

}

std::string MyException::what() const
{
    return message;
}