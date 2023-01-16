#ifndef _TOKEN_HPP
#define _TOKEN_HPP

#include <string>
#include <map>
#include <cmath>

namespace module_lexer
{
    enum Token_Type
    {
        COMMENT, //注释
        ID,

        ORIGIN,
        SCALE,
        ROT,
        IS,
        TO,
        STEP,
        DRAW,
        FOR,
        FROM, //关键字

        T, //参数

        SEMICO,    //分号
        L_BRACKET, //左括号
        R_BRACKET, //右括号
        COMMA,     //分隔符

        PLUS,
        MINUS,
        MUL,
        DIV,
        POWER, //运算符

        FUNC,     //函数名
        CONST_ID, //常数

        NONTOKEN, //输入结束(#)
        ERRTOKEN  //错误记号
    };

    typedef double (*PFunc)(double);
    // typedef unsigned int uint;

    class Token
    {
    public:
        struct BaseToken
        {
            Token_Type type;
            std::string lexeme;
            double value;
            PFunc funcPtr;

            void baseClear()
            {
                type = ERRTOKEN;
                lexeme = "";
                value = 0.0f;
                funcPtr = nullptr;
            }

            BaseToken()
            {
                baseClear();
            }

            BaseToken(Token_Type type, std::string lexeme, double value, PFunc funcPtr)
            {
                this->type = type;
                this->lexeme = lexeme;
                this->value = value;
                this->funcPtr = funcPtr;
            }
        };

        Token();
        Token(const BaseToken &base);

        void clear();
        bool isEmpty();
        void setBase(const BaseToken &b);
        void setType(const Token_Type t);
        void setValue(const double v);
        void setText(const std::string &s);
        // void setLine(uint n);
        // void setColumn(uint m);
        void append(char ch);
        void append(std::string s);

        Token_Type getType() const { return baseToken.type; }
        double getValue() const { return baseToken.value; }
        std::string getText() const { return baseToken.lexeme; }
        PFunc getFuncPtr() const { return baseToken.funcPtr; }
        // uint getLine() const { return line; }
        // uint getColumn() const { return column; }
        std::string getUpperText();

        static std::string stringToUpper(const std::string &s);

    private:
        BaseToken baseToken;
        // uint line;     //行号
        // uint column;   //列号
    };
}

#endif