#include "StateVariable.h"

StateVariable::StateVariable() : Object::Object()
{
    //ctor
}

StateVariable::~StateVariable()
{
    //dtor
}

StateVariable::StateVariable(const StateVariable& other) :Object::Object(other)
{
    //copy ctor
}

StateVariable& StateVariable::operator=(const StateVariable& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    Object::operator=(rhs);
    return *this;
}

void StateVariable::Renew()
{
    SetValue(GetValue(Expression::timing::present),Expression::timing::past);
}
