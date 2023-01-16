#include "lexer/token.hpp"

namespace module_lexer
{
    static int getStartState()
    {
        return 0;
    }

    static Token_Type getFinalState(int state)
    {
        switch (state)
        {
        case 1:
            return ID;
        case 2:
        case 3:
            return CONST_ID;
        case 4:
            return MUL;
        case 5:
            return POWER;
        case 6:
            return DIV;
        case 7:
            return MINUS;
        case 8:
            return PLUS;
        case 9:
            return COMMA;
        case 10:
            return SEMICO;
        case 11:
            return L_BRACKET;
        case 12:
            return R_BRACKET;
        case 13:
            return COMMENT;
        default:
            return ERRTOKEN;
        }
    }

    static bool isAlpha(char ch)
    {
        return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
    }

    static bool isDigit(char ch)
    {
        return (ch >= '0' && ch <= '9');
    }

    static bool isSpace(char ch)
    {
        return isspace(ch);
    }

    static int move(int state, char ch)
    {
        switch (state)
        {
        case 0:
            if (isAlpha(ch))
                return 1;
            else if (isDigit(ch))
                return 2;
            else if (ch == '*')
                return 4;
            else if (ch == '/')
                return 6;
            else if (ch == '-')
                return 7;
            else if (ch == '+')
                return 8;
            else if (ch == ',')
                return 9;
            else if (ch == ';')
                return 10;
            else if (ch == '(')
                return 11;
            else if (ch == ')')
                return 12;
            break;

        case 1:
            if (isAlpha(ch) || isDigit(ch))
                return 1;
            break;

        case 2:
            if (ch == '.')
                return 3;
            else if (isDigit(ch))
                return 2;
            break;

        case 3:
            if (isDigit(ch))
                return 3;
            break;

        case 4:
            if (ch == '*')
                return 5;
            break;

        case 6:
            if (ch == '/')
                return 13;
            break;

        case 7:
            if (ch == '-')
                return 13;
            break;

        default:
            return -1;
            break;
        }
        return -1;
    }
}
