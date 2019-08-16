#ifndef CONTROLPARAMETER_H
#define CONTROLPARAMETER_H

#include "string.h"
#include "Object.h"

class ControlParameter:public Object
{
    public:
        ControlParameter();
        virtual ~ControlParameter();
        ControlParameter(const ControlParameter& other);
        ControlParameter& operator=(const ControlParameter& other);

    protected:

    private:


};

#endif // CONTROLPARAMETER_H
