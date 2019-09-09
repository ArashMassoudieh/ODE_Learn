#ifndef STATEVARIABLE_H
#define STATEVARIABLE_H

#include "Object.h"
#include "Expression.h"

class StateVariable : public Object
{
    public:
        StateVariable();
        virtual ~StateVariable();
        StateVariable(const StateVariable& other);
        StateVariable& operator=(const StateVariable& other);
        double CalcRateOfChange();
        void Renew();
        bool SetRateOfChange(const Expression &exp) {rateofchange = exp;}
        Expression *RateOfChange() {return &rateofchange;}
        double GetRateOfChange(Expression::timing tmg) {return rateofchange.calc(Parent(),tmg);}
    protected:

    private:
        Expression rateofchange;
};

#endif // STATEVARIABLE_H
