#ifndef EXTERNALFORCING_H
#define EXTERNALFORCING_H

#include "Object.h"

class ExternalForcing: public Object
{
    public:
        ExternalForcing();
        virtual ~ExternalForcing();
        ExternalForcing(const ExternalForcing& other);
        ExternalForcing& operator=(const ExternalForcing& other);

    protected:

    private:
};

#endif // EXTERNALFORCING_H
