#ifndef DLIB_WRAPPER_H
#define DLIB_WRAPPER_H
#include <iostream>
#include <dlib/mlp.h>

using namespace std;

class dlib_wrapper
{
    public:
        dlib_wrapper();
        virtual ~dlib_wrapper();
        dlib_wrapper(const dlib_wrapper& other);
        dlib_wrapper& operator=(const dlib_wrapper& other);

    protected:

    private:
};

#endif // DLIB_WRAPPER_H
