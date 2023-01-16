#ifndef _LEXER_HPP
#define _LEXER_HPP

#include "token.hpp"
#include <string>
#include <fstream>
#include <iostream>

namespace module_lexer
{
    typedef unsigned int uint;
    class Lexer
    {
    public:
        Lexer();
        ~Lexer();

        bool initScanner(std::string fileName);
        void closeScanner();
        char getChar();
        void BackChar(char c);
        Token getToken();
        Token::BaseToken *JudgeKeyToken(std::string s);

        int pre_process(Token *pToken);
        int scan_move(Token *pToken, int first_char);
        int post_process(Token *pToken, int last_state);

        uint getLine() const { return line; }
        uint getColumn() const { return column; }

        void clear();

    private:
        uint line;
        uint column;
        std::ifstream fileStream;
        std::string fileName;
    };

}

#endif