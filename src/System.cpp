#include "System.h"

System::System()
{
    //ctor
}

System::~System()
{
    //dtor
}

System::System(const System& other)
{
    //copy ctor
}

System& System::operator=(const System& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
