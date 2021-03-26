#include "StateValuePairs.h"

StateValuePairs::StateValuePairs()
{
    //ctor
}

StateValuePairs::~StateValuePairs()
{
    //dtor
}

StateValuePairs::StateValuePairs(const StateValuePairs& other)
{
    statevalues = other.statevalues;
}

StateValuePairs& StateValuePairs::operator=(const StateValuePairs& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    statevalues = rhs.statevalues;
    return *this;
}
