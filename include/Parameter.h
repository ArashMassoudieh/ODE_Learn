#ifndef PARAMETER_H
#define PARAMETER_H

#include <Object.h>


class Parameter : public Object
{
    public:
        Parameter();
        virtual ~Parameter();
        Parameter(const Parameter& other);
        Parameter& operator=(const Parameter& other);
    protected:

    private:
};

#endif // PARAMETER_H
