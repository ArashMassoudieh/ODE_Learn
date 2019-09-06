#include "System.h"
#include <iostream>
#include "Vector_arma.h"
#include "Matrix_arma.h"

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
        return exforce(param)->Object::GetValue(tmg);
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

bool System::OneStepSolve(double dt)
{
    Renew();

    CVector_arma X = GetStateVariables(Expression::timing::past);
    CVector_arma X_past = X;
    CVector_arma F = GetResiduals(X);

    double err_ini = F.norm2();
    double err;
    double err_p = err = err_ini;
    SolverTempVars.numiterations = 0;
    bool switchvartonegpos = true;
    int attempts = 0;
    while (attempts<2 && switchvartonegpos)
    {
        while (err/err_ini>SolverSettings.NRtolerance && err>1e-12)
        {
            SolverTempVars.numiterations++;
            if (SolverTempVars.updatejacobian)
            {
                SolverTempVars.Inverse_Jacobian = Invert(Jacobian(X));
                SolverTempVars.updatejacobian = false;
                SolverTempVars.NR_coefficient = 1;
            }
            X = X - SolverTempVars.NR_coefficient*SolverTempVars.Inverse_Jacobian*F;
            F = GetResiduals(variable, X);
            err_p = err;
            err = F.norm2();
            #ifdef Debug_mode
            //ShowMessage(numbertostring(err));
            #endif // Debug_mode
            if (err>err_p)
                SolverTempVars.NR_coefficient*=SolverSettings.NR_coeff_reduction_factor;
            //else
            //    SolverTempVars.NR_coefficient/=SolverSettings.NR_coeff_reduction_factor;
            if (SolverTempVars.numiterations>SolverSettings.NR_niteration_max)
                return false;
        }
        switchvartonegpos = false;
        for (unsigned int i=0; i<blocks.size(); i++)
        {
            if (X[i]<-1e-13 && !blocks[i].GetLimitedOutflow())
            {
                blocks[i].SetLimitedOutflow(true);
                switchvartonegpos = true;
                SolverTempVars.updatejacobian = true;
            }
            else if (X[i]>1 && blocks[i].GetLimitedOutflow())
            {
                blocks[i].SetLimitedOutflow(false);
                switchvartonegpos = true;
                SolverTempVars.updatejacobian = true;
            }
        }
    }

	#ifdef Debug_mode
//	CMatrix_arma M = Jacobian("Storage",X);
//	M.writetofile("M.txt");
	#endif // Debug_mode
	return true;
}

bool System::Renew()
{
	bool out = true;
	for (unsigned int i = 0; i < statevariables.size(); i++)
		statevariables[i].Renew();

	return out;
}

CVector_arma System::GetStateVariables(Expression::timing tmg)
{
    CVector_arma out;
    for (unsigned int i = 0; i < statevariables.size(); i++)
    {
        if (tmg == Expression::timing::past)
            out.append(statevariables[i].value_past);
        else
            out.append(statevariables[i].value_current);
    }
    return out;
}

void System::SetStateVariables(CVector_arma &X,Expression::timing tmg)
{
    for (unsigned int i = 0; i < statevariables.size(); i++)
    {
        if (tmg == Expression::timing::past)
            statevariables[i].value_past = X[i];
        else
            statevariables[i].value_current = X[i];
    }
    return out;
}

CVector_arma System::GetResiduals(CVector_arma &X)
{
    CVector_arma F(statevariables.size());
    SetStateVariables(X,Expression::timing::present);
    CalculateFlows(Variable(variable)->GetCorrespondingFlowVar(),Expression::timing::present);

    for (unsigned int i=0; i<blocks.size(); i++)
    {
        if (blocks[i].GetLimitedOutflow())
        {
            blocks[i].SetOutflowLimitFactor(X[i]);
            blocks[i].SetVal(variable,0);
            F[i] = (0-blocks[i].GetVal(variable,Expression::timing::past))/dt() - blocks[i].GetInflowValue(variable,Expression::timing::present);
        }
        else
            F[i] = (X[i]-blocks[i].GetVal(variable,Expression::timing::past))/dt() - blocks[i].GetInflowValue(variable,Expression::timing::present);
    }


    for (unsigned int i=0; i<links.size(); i++)
    {
        if (blocks[links[i].s_Block_No()].GetLimitedOutflow() && links[i].GetVal(blocks[links[i].s_Block_No()].Variable(variable)->GetCorrespondingFlowVar(),Expression::timing::present)>0)
            links[i].SetOutflowLimitFactor(blocks[links[i].s_Block_No()].GetOutflowLimitFactor());
        if (blocks[links[i].e_Block_No()].GetLimitedOutflow() && links[i].GetVal(blocks[links[i].e_Block_No()].Variable(variable)->GetCorrespondingFlowVar(),Expression::timing::present)<0)
            links[i].SetOutflowLimitFactor(blocks[links[i].e_Block_No()].GetOutflowLimitFactor());

    }

    for (unsigned int i=0; i<links.size(); i++)
    {
        F[links[i].s_Block_No()] += links[i].GetVal(blocks[links[i].s_Block_No()].Variable(variable)->GetCorrespondingFlowVar(),Expression::timing::present)*links[i].GetOutflowLimitFactor();
        F[links[i].e_Block_No()] -= links[i].GetVal(blocks[links[i].s_Block_No()].Variable(variable)->GetCorrespondingFlowVar(),Expression::timing::present)*links[i].GetOutflowLimitFactor();
    }
    return F;
}
