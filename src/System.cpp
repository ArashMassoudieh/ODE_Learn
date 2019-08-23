#include "System.h"
#include <iostream>

using namespace std;
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

Object* System::object(const string &s)
{
    for (int i=0; i<statevariables.size(); i++)
        if (statevariables[i].GetName() == s)
            return &statevariables[i];

    for (int i=0; i<externalforcings.size(); i++)
        if (externalforcings[i].GetName() == s)
            return &externalforcings[i];

    for (int i=0; i<controlparameters.size(); i++)
        if (controlparameters[i].GetName() == s)
            return &controlparameters[i];

    return nullptr;
}

object_type System::GetType(const string &param)
{
    for (int i=0; i<statevariables.size(); i++)
        if (statevariables[i].GetName() == param)
            return object_type::state;

    for (int i=0; i<externalforcings.size(); i++)
        if (externalforcings[i].GetName() == param)
            return object_type::exforce;

    for (int i=0; i<controlparameters.size(); i++)
        if (controlparameters[i].GetName() == param)
            return object_type::control;

    return object_type::not_found;
}

StateVariable* System::state(const string &s)
{
    for (int i=0; i<statevariables.size(); i++)
        if (statevariables[i].GetName() == s)
            return &statevariables[i];

    return nullptr;
}

ControlParameter* System::control(const string &s)
{
    for (int i=0; i<controlparameters.size(); i++)
        if (controlparameters[i].GetName() == s)
            return &controlparameters[i];

    return nullptr;
}

ExternalForcing* System::exforce(const string &s)
{
    for (int i=0; i<externalforcings.size(); i++)
        if (externalforcings[i].GetName() == s)
            return &externalforcings[i];
    return nullptr;
}

double System::GetValue(const string &param, Expression::timing tmg)
{
    if (GetType(param) == object_type::state)
        return state(param)->GetValue(tmg);

    if (GetType(param) == object_type::control)
        return control(param)->GetValue(tmg);

    if (GetType(param) == object_type::exforce)
        return exforce(param)->GetValue(tmg);
}

bool System::AppendState(const StateVariable &stt)
{
    if (object(stt.GetName())!=nullptr)
    {
        cout<<"Object '" + stt.GetName() + "' already exists!";
        return false;
    }
    else
    {
        statevariables.push_back(stt);
        state(stt.GetName())->SetParent(this);
        return true;
    }

}

bool System::AppendControlParameter(const ControlParameter &ctr)
{
    if (object(ctr.GetName())!=nullptr)
    {
        cout<<"Object '" + ctr.GetName() + "' already exists!";
        return false;
    }
    else
    {
        controlparameters.push_back(ctr);
        control(ctr.GetName())->SetParent(this);
        return true;
    }
}

bool System::AppendExternalForcing(const ExternalForcing &extforce)
{
    if (object(extforce.GetName())!=nullptr)
    {
        cout<<"Object '" + extforce.GetName() + "' already exists!";
        return false;
    }
    else
    {
        externalforcings.push_back(extforce);
        exforce(extforce.GetName())->SetParent(this);
        return true;
    }
}
