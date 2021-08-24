#ifndef STATEVARIABLE_H
#define STATEVARIABLE_H

#include "Object.h"
#include "Expression.h"
#include "Vector.h"

class StateVariable : public Object
{
    public:
        StateVariable();
        virtual ~StateVariable();
        StateVariable(const StateVariable& other);
        StateVariable& operator=(const StateVariable& other);
        double CalcRateOfChange();
        void Renew();
        bool SetRateOfChange(const Expression &exp) {rateofchange = exp;return true;}
        Expression *RateOfChange() {return &rateofchange;}
        double GetRateOfChange(Expression::timing tmg)
        {
            double out = rateofchange.calc(Parent(),tmg);
            return out;
        }
        CVector GradientvsControlParameters(const double &dt);
    protected:

    private:
        Expression rateofchange;
};

#endif // STATEVARIABLE_H
