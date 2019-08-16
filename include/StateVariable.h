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

    protected:

    private:
        Expression rateofchange;
};

#endif // STATEVARIABLE_H
