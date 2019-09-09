#include "Parameter.h"

Parameter::Parameter()
{
    //ctor
}

Parameter::~Parameter()
{
    //dtor
}

Parameter& Parameter::operator=(const Parameter& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    Object::operator=(rhs);
    return *this;
}

Parameter::Parameter(const Parameter& other) :Object::Object(other)
{
    //copy ctor
}

