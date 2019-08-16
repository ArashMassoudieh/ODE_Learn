#ifndef SYSTEM_H
#define SYSTEM_H

#include "StateVariable.h"
#include "ControlParameter.h"
#include "ExternalForcing.h"
#include <vector>

using namespace std;

class System
{
    public:
        System();
        virtual ~System();
        System(const System& other);
        System& operator=(const System& other);

    protected:

    private:
        vector<StateVariable> statevariables;
        vector<ControlParameter> controlparameters;
        vector<ExternalForcing> externalforcing;

};

#endif // SYSTEM_H
