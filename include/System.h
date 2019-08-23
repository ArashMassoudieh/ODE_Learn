#ifndef SYSTEM_H
#define SYSTEM_H

#include "StateVariable.h"
#include "ControlParameter.h"
#include "ExternalForcing.h"
#include <vector>

using namespace std;

enum class object_type {state, control, exforce, not_found};

class System
{
    public:
        System();
        virtual ~System();
        System(const System& other);
        System& operator=(const System& other);
        Object *object(const string &s);
        StateVariable *state(const string &s);
        ControlParameter *control(const string &s);
        ExternalForcing *exforce(const string &s);
        double GetValue(const string &param, Expression::timing tmg=Expression::timing::present);
        object_type GetType(const string &param);
        bool AppendState(const StateVariable &stt);
        bool AppendControlParameter(const ControlParameter &ctr);
        bool AppendExternalForcing(const ExternalForcing &extforce);
    protected:

    private:
        vector<StateVariable> statevariables;
        vector<ControlParameter> controlparameters;
        vector<ExternalForcing> externalforcings;

};

#endif // SYSTEM_H
