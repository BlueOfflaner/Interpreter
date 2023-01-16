#include "parser/parser.hpp"

#define _INCLUDE_SEMANTICS

#ifdef _INCLUDE_SEMANTICS
#include "semantics/semantics.hpp"
#endif

using namespace module_lexer;
using namespace std;

namespace module_parser
{
    Parser::Parser()
    {
        clear();
    }

    Parser::Parser(string fileName)
    {
        clear();
        initParser(fileName);
    }

    Parser::~Parser()
    {
        closeParser();
        free(this);
    }

    void Parser::clear()
    {
        lexer.clear();
        //indent = 0;
        start = nullptr;
        end = nullptr;
        x = nullptr;
        y = nullptr;
        angle = nullptr;
        step = nullptr;
        currentToken.clear();
        destoryAllTree();
    }

    void Parser::initParser(string fileName)
    {
        clear();
        lexer.initScanner(fileName);
        semanticAnalyzer.initSemanticAnalyzer(&parameter);
    }

    void Parser::closeParser()
    {
        lexer.~Lexer();
        destoryAllTree();
    }

    void Parser::run()
    {
        fetchToken();
        program();
    }

    void Parser::program()
    {
        while (currentToken.getType() != NONTOKEN)
        {
            statement();
            matchToken(SEMICO);
        }
    }

    void Parser::statement()
    {
        switch (currentToken.getType())
        {
        case ORIGIN:
            originStatement();
            break;
        case ROT:
            rotStatement();
            break;
        case SCALE:
            scaleStatement();
            break;
        case FOR:
            forStatement();
            break;
        default:
            reportSyntaxError(2);
            break;
        }

        destoryAllTree();
    }

    void Parser::originStatement()
    {
        matchToken(ORIGIN);
        matchToken(IS);
        matchToken(L_BRACKET);
        {
            x = expression();
            printTree(x);
        }
        matchToken(COMMA);
        {
            y = expression();
            printTree(y);
        }
        matchToken(R_BRACKET);

        #ifdef _INCLUDE_SEMANTICS
        semanticAnalyzer.setOrigin(x, y);
        //语义计算
        #endif
    }

    void Parser::rotStatement()
    {
        matchToken(ROT);
        matchToken(IS);
        {
            angle = expression();
            printTree(angle);
        }

        #ifdef _INCLUDE_SEMANTICS
        semanticAnalyzer.setRot(angle);
        //语义计算
        #endif        
    }

    void Parser::scaleStatement()
    {
        matchToken(SCALE);
        matchToken(IS);
        matchToken(L_BRACKET);
        {
            x = expression();
            printTree(x);
        }
        matchToken(COMMA);
        {
            y = expression();
            printTree(y);
        }
        matchToken(R_BRACKET);

        #ifdef _INCLUDE_SEMANTICS
        semanticAnalyzer.setScale(x, y);
        //语义计算
        #endif
    }

    void Parser::forStatement()
    {
        matchToken(FOR);
        matchToken(T);
        matchToken(FROM);
        {
            start = expression();
            printTree(start);
        }
        matchToken(TO);
        {
            end = expression();
            printTree(end);
        }
        matchToken(STEP);
        {
            step = expression();
            printTree(step);
        }
        matchToken(DRAW);
        matchToken(L_BRACKET);
        {
            x = expression();
            printTree(x);
        }
        matchToken(COMMA);
        {
            y = expression();
            printTree(y);
        }
        matchToken(R_BRACKET);

        #ifdef _INCLUDE_SEMANTICS
        semanticAnalyzer.loop(start, end, step, x, y);
        //语义计算
        #endif
    }

    ExprNode *Parser::expression()
    {
        ExprNode *left, *right;
        left = term();
        Token_Type tmp;
        while(currentToken.getType() == PLUS || currentToken.getType() == MINUS)
        {
            tmp = currentToken.getType();
            matchToken(currentToken.getType());
            right = term();
            left = makeExprNode(tmp, ExprNode::content::caseOperator{left,right});
        }
        return left;
    }

    ExprNode *Parser::term()
    {
        ExprNode *left, *right;
        left = factor();
        Token_Type tmp;
        while(currentToken.getType() == MUL || currentToken.getType() == DIV)
        {
            tmp = currentToken.getType();
            matchToken(currentToken.getType());
            right = factor();
            left = makeExprNode(tmp, ExprNode::content::caseOperator{left,right});
        }
        return left;
    }

    ExprNode *Parser::factor()
    {
        ExprNode *left, *right;
        if(currentToken.getType() == PLUS)
        {
            matchToken(PLUS);
            right = factor();
        }
        else if(currentToken.getType() == MINUS)
        {
            matchToken(MINUS);
            right = factor();
            left = (ExprNode*)malloc(sizeof(ExprNode));
            left->OpCode = CONST_ID;
            left->Content.CaseConst = 0.0f;
            right = makeExprNode(MINUS, ExprNode::content::caseOperator{left,right});
        }
        else right = component();

        return right;
    }

    ExprNode *Parser::component()
    {
        ExprNode *left, *right;
        left = atom();
        if(currentToken.getType() == POWER)
        {
            matchToken(POWER);
            right = component();
            left = makeExprNode(POWER, ExprNode::content::caseOperator{left,right});
        }
        return left;
    }

    ExprNode *Parser::atom()
    {
        Token tmp = currentToken;
        ExprNode *root;
        switch (currentToken.getType())
        {
        case CONST_ID:
            matchToken(CONST_ID);
            root = makeExprNode(CONST_ID, tmp.getValue());
            break;
        case T:
            matchToken(T);
            root = makeExprNode(T);
            break;
        case FUNC:
            matchToken(FUNC);
            matchToken(L_BRACKET);
            root = makeExprNode(FUNC, ExprNode::content::caseFunc{expression(), tmp.getFuncPtr()});
            matchToken(R_BRACKET);
            break;
        case L_BRACKET:
            matchToken(L_BRACKET);
            root = expression();
            matchToken(R_BRACKET);
            break;
        default:
            reportSyntaxError(2);
            break;
        }
        return root;
    }

    void Parser::fetchToken()
    {
        currentToken = lexer.getToken();
        if(currentToken.getType() == ERRTOKEN) 
        {
            reportSyntaxError(1);
        }
    }

    void Parser::matchToken(Token_Type type)
    {
        if(currentToken.getType() != type)
        {
            reportSyntaxError(2);
        }
        fetchToken();
    }

    void errorMessage(unsigned int line, std::string token, std::string errorType)
    {
        std::string msg = "";
        msg = "Error in line " + std::to_string(line) + ": " + errorType + " " + token + "\n";
        try
        {
            throw MyException(msg);
        }
        catch(MyException& e)
        {
            cout << e.what() << endl;
            closeLog();
            exit(1);
        }
    }

    void Parser::reportSyntaxError(int code)
    {
        switch(code)
        {
        case 1:
            errorMessage(lexer.getLine(), currentToken.getText(), "非法单词");
            break;
        case 2:
            errorMessage(lexer.getLine(), currentToken.getText(), "非预期记号");
            break;
        }
        getchar();
        exit(1);
    }

    void Parser::printTree(ExprNode* root)
    {
        //cout << "Expression Tree:\n";
        logTree("Expression Tree\n");
        printSyntaxTree(root, 1);
        logTree("\n\n");
        //cout << "\n\n";
    }

    map<Token_Type, string> table = {
        {PLUS, "+"},
        {MINUS, "-"},
        {MUL, "*"},
        {DIV, "/"},
        {POWER, "**"},
    };

    void Parser::printSyntaxTree(ExprNode* root, int indent)
    {
        
        for(int i = 0; i < indent; i++)
        {
            logTree("\t");
        } //cout << "\t";
        logTree("|——  ");
        if(root->OpCode == FUNC)
        {
            logTree("%p\n", (root->Content.CaseFunc.mathFuncPtr));
            //cout << reinterpret_cast<void*>(root->Content.CaseFunc.mathFuncPtr) << "\n";
            printSyntaxTree(root->Content.CaseFunc.child, indent+1);
        }
        else if(root->OpCode == T)
        {
            logTree("T\n");
            //cout << "T\n";
        }
        else if(root->OpCode == CONST_ID)
        {
            logTree("%lf\n", root->Content.CaseConst);
            //cout << root->Content.CaseConst << "\n";
        }
        else
        {
            if(table.count(root->OpCode) > 0)
            {
                logTree("%s\n", table[root->OpCode].c_str());
                //cout << table[root->OpCode] << "\n";
                printSyntaxTree(root->Content.CaseOperator.left, indent+1);
                printSyntaxTree(root->Content.CaseOperator.right, indent+1);
            }
            else
            {
                logTree("illegal node\n");
                //cout << "illegal node\n";
            }
        }
    }

    void Parser::destoryTree(ExprNode* root)
    {
        if(root == nullptr) return;
        switch (root->OpCode)
        {
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case POWER:
            destoryTree(root->Content.CaseOperator.left);
            destoryTree(root->Content.CaseOperator.right);
            break;
        case FUNC:
            destoryTree(root->Content.CaseFunc.child);
            break;
        default:
            break;
        }

        free(root);
    }

    void Parser::destoryAllTree()
    {
        destoryTree(start); start = nullptr;
        destoryTree(end); end = nullptr;
        destoryTree(x); x = nullptr;
        destoryTree(y); y = nullptr;
        destoryTree(step); step = nullptr;
        destoryTree(angle); angle = nullptr;
    }

    ExprNode *Parser::makeExprNode(Token_Type opcode)
    {
        ExprNode *ptr = (ExprNode*)malloc(sizeof(ExprNode));
        ptr->OpCode = opcode;
        ptr->Content.CaseParmPtr = &(parameter);
        return ptr;
    }

    ExprNode *Parser::makeExprNode(Token_Type opcode, double caseConst)
    {
        ExprNode *ptr = (ExprNode*)malloc(sizeof(ExprNode));
        ptr->OpCode = opcode;
        ptr->Content.CaseConst = caseConst;
        return ptr;
    }

    ExprNode *Parser::makeExprNode(Token_Type opcode, ExprNode::content::caseFunc caseFunc)
    {
        ExprNode *ptr = (ExprNode*)malloc(sizeof(ExprNode));
        ptr->OpCode = opcode;
        ptr->Content.CaseFunc = caseFunc;
        return ptr;
    }

    ExprNode *Parser::makeExprNode(Token_Type opcode, ExprNode::content::caseOperator caseOperator)
    {
        ExprNode *ptr = (ExprNode*)malloc(sizeof(ExprNode));
        ptr->OpCode = opcode;
        ptr->Content.CaseOperator = caseOperator;
        return ptr;
    }
}