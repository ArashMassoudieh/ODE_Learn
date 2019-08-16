#include "ExternalForcing.h"

ExternalForcing::ExternalForcing(): Object::Object()
{
    //ctor
}

ExternalForcing::~ExternalForcing()
{
    //dtor
}

ExternalForcing::ExternalForcing(const ExternalForcing& other):Object::Object(other)
{
    //copy ctor
}

ExternalForcing& ExternalForcing::operator=(const ExternalForcing& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    Object::operator=(rhs);
    return *this;
}
