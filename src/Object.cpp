#include "Object.h"

Object::Object()
{
    //ctor
}

Object::~Object()
{
    //dtor
}

Object::Object(const Object& other)
{
    name = other.name;
    value_current = other.value_current;
    value_past = other.value_past;
    parent = other.parent;

}

Object& Object::operator=(const Object& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    name = rhs.name;
    value_current = rhs.value_current;
    value_past = rhs.value_past;
    parent = rhs.parent;
    //assignment operator
    return *this;
}

void Object::SetParent(System *system)
{
    parent = system;
}
