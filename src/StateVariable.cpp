#include "StateVariable.h"
#include "System.h"

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
    rateofchange = rhs.rateofchange;
    return *this;
}

void StateVariable::Renew()
{
    SetValue(GetValue(Expression::timing::present),Expression::timing::past);
}

CVector StateVariable::GradientvsControlParameters(const double &dt)
{
    CVector gradient(Parent()->Count(object_type::control));
    double original_rateofchange = rateofchange.calc(Parent(),Expression::timing::past);
    for (unsigned int i=0; i<Parent()->Count(object_type::control); i++)
    {
        double original_control_value = Parent()->control(i)->GetValue(Expression::timing::past);
        Parent()->control(i)->SetValue(original_control_value+EPSILON,Expression::timing::past);
        double new_rateofchange = rateofchange.calc(Parent(),Expression::timing::past);
        gradient[i] = (new_rateofchange-original_rateofchange)/EPSILON*dt;
        Parent()->control(i)->SetValue(original_control_value,Expression::timing::past);
    }
    return gradient;
}

