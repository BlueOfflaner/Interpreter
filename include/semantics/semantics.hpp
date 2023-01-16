#ifndef _SEMANTICS_HPP
#define _SEMANTICS_HPP

#include "../lexer/token.hpp"
#include "../parser/exprNode.hpp"

namespace module_semantics
{
    class SemanticAnalyzer
    {
    public:
        SemanticAnalyzer();

        void clear();
        void initSemanticAnalyzer(double *param);

        void setOrigin(ExprNode *x, ExprNode *y);
        void setScale(ExprNode *x, ExprNode *y);
        void setRot(ExprNode *angle);
        
        double calcExprValue(ExprNode *root);
        void calcPointPos(ExprNode *xTree, ExprNode *yTree, double &valueX, double &valueY);
        void loop(ExprNode *start, ExprNode *end, ExprNode *step, ExprNode *x, ExprNode *y);
    private:
        double originX, originY;
        double scaleX, scaleY;
        double rotAngle;
        double *parameter;
    };
}

#endif