#include "ControlParameter.h"

ControlParameter::ControlParameter(): Object::Object()
{
    //ctor
}

ControlParameter::~ControlParameter()
{
    //dtor
}

ControlParameter::ControlParameter(const ControlParameter& other):Object::Object(other)
{
    //copy ctor
}

ControlParameter& ControlParameter::operator=(const ControlParameter& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    Object::operator=(rhs);
    return *this;
}
