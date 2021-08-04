#include "DynamicValue.h"

DynamicValue::DynamicValue() : Object::Object()
{
    //ctor
}

DynamicValue::~DynamicValue()
{
    //dtor
}

DynamicValue::DynamicValue(const DynamicValue& other):Object::Object(other)
{
    expression = other.expression;
}

DynamicValue& DynamicValue::operator=(const DynamicValue& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    Object::operator=(rhs);
    expression=rhs.expression;
    return *this;
}

bool DynamicValue::SetExpression(const Expression &exp)
{
    expression=exp;
    return true;
}


