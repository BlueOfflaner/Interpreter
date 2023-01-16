#include "semantics/semantics.hpp"
#include "ui_main/GUI.hpp"
#include <cmath>

#define l_value calcExprValue(root->Content.CaseOperator.left)
#define r_value calcExprValue(root->Content.CaseOperator.right)

using namespace module_lexer;

namespace module_semantics
{
    SemanticAnalyzer::SemanticAnalyzer()
    {
        clear();
    }

    void SemanticAnalyzer::clear()
    {
        originX = 0;
        originY = 0;
        scaleX = 0;
        scaleY = 0;
        rotAngle = 0;
    }

    void SemanticAnalyzer::initSemanticAnalyzer(double *param)
    {
        clear();
        this->parameter = param;
    }

    void SemanticAnalyzer::setOrigin(ExprNode *x, ExprNode *y)
    {
        originX = calcExprValue(x);
        originY = calcExprValue(y);
    }

    void SemanticAnalyzer::setScale(ExprNode *x, ExprNode *y)
    {
        scaleX = calcExprValue(x);
        scaleY = calcExprValue(y);
    }

    void SemanticAnalyzer::setRot(ExprNode *angle)
    {
        rotAngle = calcExprValue(angle);
    }

    double SemanticAnalyzer::calcExprValue(ExprNode *root)
    {
        if(root == nullptr) return 0;
        switch (root->OpCode)
        {
        case CONST_ID:
            return root->Content.CaseConst;
        case T:
            return *(root->Content.CaseParmPtr);
        case FUNC:
            return ((*(root->Content.CaseFunc.mathFuncPtr))(calcExprValue(root->Content.CaseFunc.child)));
        case PLUS:
            return l_value + r_value;
        case MINUS:
            return l_value - r_value;
        case MUL:
            return l_value * r_value;
        case DIV:
            return l_value / r_value;
        case POWER:
            return pow(l_value, r_value);
        }
        return 0;
    }

    void SemanticAnalyzer::calcPointPos(ExprNode *xTree, ExprNode *yTree, double &valX, double &valY)
    {
        double x, y;
        x = calcExprValue(xTree);
        y = calcExprValue(yTree);

        x *= scaleX;
        y *= scaleY;

        double tmp;
        tmp = x*cos(rotAngle) + y*sin(rotAngle);
        y = y*cos(rotAngle) - x*sin(rotAngle);
        x = tmp;

        x += originX;
        y += originY;

        valX = x;
        valY = y;
    }

    void SemanticAnalyzer::loop(ExprNode *startTree, ExprNode *endTree, ExprNode *stepTree, ExprNode *xTree, ExprNode *yTree)
    {
        double start, end, step, x, y;
        start = calcExprValue(startTree);
        end = calcExprValue(endTree);
        step = calcExprValue(stepTree);

        for(*parameter = start; *parameter <= end; *parameter += step)
        {
            calcPointPos(xTree, yTree, x, y);

            draw(x, y);
        }
    }
}