#include "lexer/lexer.hpp"
#include <cstring>
#include "dfa.cpp"

using namespace std;

namespace module_lexer
{
    std::map<std::string, Token::BaseToken> TokenTable = {
        {"ORIGIN", Token::BaseToken(ORIGIN, "ORIGIN", 0, nullptr)},
        {"SCALE", Token::BaseToken(SCALE, "SCALE", 0, nullptr)},
        {"ROT", Token::BaseToken(ROT, "ROT", 0, nullptr)},
        {"IS", Token::BaseToken(IS, "IS", 0, nullptr)},
        {"FROM", Token::BaseToken(FROM, "FROM", 0, nullptr)},
        {"FOR", Token::BaseToken(FOR, "FOR", 0, nullptr)},
        {"TO", Token::BaseToken(TO, "TO", 0, nullptr)},
        {"STEP", Token::BaseToken(STEP, "STEP", 0, nullptr)},
        {"DRAW", Token::BaseToken(DRAW, "DRAW", 0, nullptr)},

        {"PI", Token::BaseToken(CONST_ID, "PI", 3.1415926, nullptr)},
        {"E", Token::BaseToken(CONST_ID, "E", 2.71828, nullptr)},
        {"T", Token::BaseToken(T, "T", 0, nullptr)},

        {"SIN", Token::BaseToken(FUNC, "SIN", 0, sin)},
        {"COS", Token::BaseToken(FUNC, "COS", 0, cos)},
        {"TAN", Token::BaseToken(FUNC, "TAN", 0, tan)},
        {"LN", Token::BaseToken(FUNC, "LN", 0, log)},
        {"EXP", Token::BaseToken(FUNC, "EXP", 0, exp)},
        {"SQRT", Token::BaseToken(FUNC, "SQRT", 0, sqrt)},

        // {"PLUS", Token::BaseToken(PLUS, "+", 0, nullptr)},
        // {"MINUS", Token::BaseToken(MINUS, "-", 0, nullptr)},
        // {"MUL", Token::BaseToken(MUL, "*", 0, nullptr)},
        // {"DIV", Token::BaseToken(DIV, "/", 0, nullptr)},
        // {"POWER", Token::BaseToken(POWER, "**", 0, nullptr)},
    };

    Lexer::Lexer()
    {
        clear();
    }

    Lexer::~Lexer()
    {
        closeScanner();
        free(this);
    }

    void Lexer::clear()
    {
        line = 1;
        column = 0;
        fileStream.clear();
        fileName.clear();
    }

    bool Lexer::initScanner(string fileName)
    {
        line = 1;
        column = 0;
        fileStream.open(fileName);
        if (!fileStream)
            return false;
        else
        {
            this->fileName = fileName;
            return true;
        }
    }

    void Lexer::closeScanner()
    {
        fileStream.close();
        fileStream.clear();
    }

    char Lexer::getChar()
    {
        int next_char = fileStream.get();
        if (next_char == EOF)
            return -1;
        else
        {
            if (next_char == '\n')
            {
                line++;
                column = 0;
            }
            else
            {
                column++;
            }
            return next_char;
        }
    }

    void Lexer::BackChar(char c)
    {
        if (c == EOF || c == '\n')
            return;
        fileStream.unget();
        column--;
    }

    Token Lexer::getToken()
    {
        int first_char;      //记号开始的第一个字符
        int last_state = -1; //识别记号结束时的状态
        Token theToken;      //识别到的记号对象
        int to_bo_continue;  //是否遇到注释

        do
        {

            first_char = pre_process(&theToken);
            if (first_char == -1)
            {
                theToken.setType(NONTOKEN);
                return theToken;
            }
            last_state = scan_move(&theToken, first_char);

            to_bo_continue = post_process(&theToken, last_state);
        } while (to_bo_continue != 0);

        // std::cout << "theToken   " << theToken.getText() << endl;
        return theToken;
    }

    int Lexer::pre_process(Token *pToken)
    {
        int current_char;
        memset(pToken, 0, sizeof(Token));
        while (1)
        {
            current_char = getChar();
            if (current_char == -1)
            {
                return -1;
            }
            if (!isSpace(current_char))
                break;
        }
        return current_char;
    }

    int Lexer::scan_move(Token *pToken, int first_char)
    {
        int current_state = getStartState();
        int current_char = first_char;
        int next_state;

        while (1)
        {
            next_state = move(current_state, current_char);
            if (next_state < 0)
            {
                if (current_state == 2 || current_state == 3)
                {

                    while (isAlpha(current_char) || isDigit(current_char) || (current_char == '.'))
                    {
                        current_state = -1;
                        pToken->append(current_char);
                        current_char = getChar();
                    }
                    if (current_char != -1)
                        BackChar(current_char);
                    break;
                }

                else if (current_state == 1)
                {

                    while (isAlpha(current_char) || isDigit(current_char))
                    {
                        current_state = -1;
                        pToken->append(current_char);
                        current_char = getChar();
                    }
                    if (current_char != -1)
                        BackChar(current_char);
                    break;
                }

                if (pToken->isEmpty())
                {
                    string s = "";
                    s += current_char;
                    pToken->setText(s);
                    break;
                }

                else
                {
                    BackChar(current_char);
                    break;
                }

                break;
            }

            pToken->append(current_char);

            current_state = next_state;
            current_char = getChar();
            if (current_char == -1)
                break;
        }
        return current_state;
    }

    int Lexer::post_process(Token *pToken, int last_state)
    {
        int to_bo_continue = 0;
        enum Token_Type tk = getFinalState(last_state);
        pToken->setType(tk);
        switch (tk)
        {
        case ID:
        {
            Token::BaseToken *tmp = JudgeKeyToken(pToken->getUpperText());
            if (tmp == nullptr)
            {
                pToken->setType(ID);
            }
            else
            {
                pToken->setBase(*tmp);
            }
            break;
        }
        case CONST_ID:
            pToken->setValue(atof(pToken->getText().c_str()));
            break;
        case COMMENT:
        {
            int c;
            while (1)
            {
                c = getChar();
                if (c == '\n' || c == -1)
                    break;
            }
            to_bo_continue = 1;
            break;
        }
        }
        return to_bo_continue;
    }

    Token::BaseToken *Lexer::JudgeKeyToken(string s)
    {
        if (TokenTable.count(s) > 0)
        {
            // std::cout << "TokenTable   " << TokenTable[s].lexeme << endl;
            return &(TokenTable[s]);
        }
        return nullptr;
    }

}
