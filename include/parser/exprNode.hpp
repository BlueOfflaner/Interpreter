#ifndef _EXPRNODE_HPP
#define _EXPRNODE_HPP

#include "../lexer/lexer.hpp"


struct ExprNode
{
    module_lexer::Token_Type OpCode;
    union content
    {
        struct caseOperator
        {
            ExprNode *left, *right;
        } CaseOperator;

        struct caseFunc
        {
            ExprNode *child;
            module_lexer::PFunc mathFuncPtr;
        } CaseFunc;

        double CaseConst;
        double *CaseParmPtr;
    } Content;
};

#endif