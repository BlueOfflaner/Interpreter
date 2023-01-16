#include "lexer/token.hpp"

namespace module_lexer
{
    Token::Token()
    {
        clear();
    }

    Token::Token(const Token::BaseToken &b)
    {
        baseToken = b;
    }

    void Token::clear()
    {
        baseToken.baseClear();
        // line = 0;
        // column = 0;
    }

    bool Token::isEmpty()
    {
        return (baseToken.lexeme.length() < 1);
    }

    void Token::setBase(const Token::BaseToken &b)
    {
        baseToken = b;
    }

    void Token::setType(const Token_Type t)
    {
        baseToken.type = t;
    }

    void Token::setValue(const double v)
    {
        baseToken.value = v;
    }

    void Token::setText(const std::string &s)
    {
        baseToken.lexeme = s;
    }

    void Token::append(char ch)
    {
        this->baseToken.lexeme += ch;
    }

    void Token::append(std::string s)
    {
        this->baseToken.lexeme += s;
    }

    std::string Token::getUpperText()
    {
        return stringToUpper(baseToken.lexeme);
    }

    std::string Token::stringToUpper(const std::string &s)
    {
        std::string upperString;
        for (int i = 0; i < s.size(); i++)
        {
            upperString = upperString + (char)toupper(s[i]);
        }
        return upperString;
    }
}
