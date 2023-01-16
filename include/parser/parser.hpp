#ifndef _PARSER_HPP
#define _PARSER_HPP

#include "exprNode.hpp"
#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "semantics/semantics.hpp"
#include "errorlog/logTree.hpp"
#include "errorlog/myException.hpp"

using namespace module_lexer;
using namespace module_semantics;

namespace module_parser
{
    class Parser
    {
    public:
        Parser();
        Parser(std::string fileName);
        ~Parser();

        void initParser(std::string fileName);
        void closeParser();
        void clear();
        void run();

    protected:
        void program();
        void statement();
        void originStatement();
        void rotStatement();
        void scaleStatement();
        void forStatement();

        ExprNode *expression();
        ExprNode *term();
        ExprNode *factor();
        ExprNode *component();
        ExprNode *atom();

        void fetchToken();
        void matchToken(Token_Type type);
        void printTree(ExprNode *root);
        void printSyntaxTree(ExprNode *root, int indent);
        void reportSyntaxError(int code);
        void destoryAllTree();
        void destoryTree(ExprNode *root);

        ExprNode *makeExprNode(module_lexer::Token_Type opcode, ExprNode::content::caseOperator caseOperator);
        ExprNode *makeExprNode(module_lexer::Token_Type opcode, ExprNode::content::caseFunc caseFunc);
        ExprNode *makeExprNode(module_lexer::Token_Type opcode, double caseConst);
        ExprNode *makeExprNode(module_lexer::Token_Type opcode);

    private:
        module_lexer::Lexer lexer;
        module_lexer::Token currentToken;
        module_semantics::SemanticAnalyzer semanticAnalyzer;

        double parameter;
        // int indent;
        ExprNode *start;
        ExprNode *end;
        ExprNode *x;
        ExprNode *y;
        ExprNode *step;
        ExprNode *angle;
    };
}

#endif