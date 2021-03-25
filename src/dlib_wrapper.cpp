#include "dlib_wrapper.h"

dlib_wrapper::dlib_wrapper()
{
    //ctor
}

dlib_wrapper::~dlib_wrapper()
{
    //dtor
}

dlib_wrapper::dlib_wrapper(const dlib_wrapper& other)
{
    //copy ctor
}

dlib_wrapper& dlib_wrapper::operator=(const dlib_wrapper& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
