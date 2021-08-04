#ifndef DynamicValue_H
#define DynamicValue_H

#include "Object.h"
#include "Expression.h"

class DynamicValue : public Object
{
    public:
        DynamicValue();
        virtual ~DynamicValue();
        DynamicValue(const DynamicValue& other);
        DynamicValue& operator=(const DynamicValue& other);
        bool SetExpression(const Expression &exp);
        double GetValue(Expression::timing tmg=Expression::timing::present)
        {
            double out = expression.calc(Parent(),tmg);
            return out;
        }

    protected:

    private:
        Expression expression;
};

#endif // REWARD_H
