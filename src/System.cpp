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

    for (int i=0; i<parameters.size(); i++)
        if (parameters[i].GetName() == s)
            return &parameters[i];

    return nullptr;
}

void System::SetAllParents()
{
    for (int i=0; i<statevariables.size(); i++)
        statevariables[i].SetParent(this);

    for (int i=0; i<externalforcings.size(); i++)
        externalforcings[i].SetParent(this);

    for (int i=0; i<controlparameters.size(); i++)
        controlparameters[i].SetParent(this);

    for (int i=0; i<parameters.size(); i++)
        parameters[i].SetParent(this);

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

    for (int i=0; i<parameters.size(); i++)
        if (parameters[i].GetName() == param)
            return object_type::parameter;

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

Parameter* System::parameter(const string &s)
{
    for (int i=0; i<parameters.size(); i++)
        if (parameters[i].GetName() == s)
            return &parameters[i];
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

    if (GetType(param) == object_type::parameter)
        return parameter(param)->Object::GetValue(tmg);
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

bool System::AppendParameter(const Parameter &param)
{
    if (object(param.GetName())!=nullptr)
    {
        cout<<"Object '" + param.GetName() + "' already exists!";
        return false;
    }
    else
    {
        parameters.push_back(param);
        parameter(param.GetName())->SetParent(this);
        return true;
    }
}


bool System::OneStepSolve(double dt)
{
    CVector_arma X_past = GetStateVariables(Expression::timing::past);
    Renew();

    CVector_arma X = GetStateVariables(Expression::timing::past);
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
                CMatrix_arma M = Jacobian(X);
                SolverTempVars.Inverse_Jacobian = Invert(M);
                SolverTempVars.updatejacobian = false;
                SolverTempVars.NR_coefficient = 1;
            }
            X = X - SolverTempVars.NR_coefficient*SolverTempVars.Inverse_Jacobian*F;
            F = GetResiduals(X);
            err_p = err;
            err = F.norm2();

            #ifdef Debug_mode
                ShowMessage(numbertostring(err));
            #endif // Debug_mode
            if (err>err_p)
            {
                SolverTempVars.NR_coefficient*=SolverSettings.NR_coeff_reduction_factor;
                X = X_past;
                F = GetResiduals(X);
            }                //else
            //    SolverTempVars.NR_coefficient/=SolverSettings.NR_coeff_reduction_factor;
            if (SolverTempVars.numiterations>SolverSettings.NR_niteration_max)
            {
                SetStateVariables(X_past,Expression::timing::past);
                return false;
            }
        }
        switchvartonegpos = false;
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
            out.append(statevariables[i].GetValue(Expression::timing::past));
        else
            out.append(statevariables[i].GetValue(Expression::timing::present));
    }
    return out;
}

void System::SetStateVariables(CVector_arma &X,Expression::timing tmg)
{
    for (unsigned int i = 0; i < statevariables.size(); i++)
    {
        if (tmg == Expression::timing::past)
            statevariables[i].SetValue(X[i], Expression::timing::past);
        else
            statevariables[i].SetValue(X[i],Expression::timing::present);
    }

}

CVector_arma System::GetResiduals(CVector_arma &X)
{
    CVector_arma F(statevariables.size());
    SetStateVariables(X,Expression::timing::present);

    for (unsigned int i=0; i<statevariables.size(); i++)
    {
            F[i] = (X[i]-statevariables[i].GetValue(Expression::timing::past))/dt() - statevariables[i].GetRateOfChange(Expression::timing::present);
    }

    return F;
}

CMatrix_arma System::Jacobian(CVector_arma &X)
{
    CMatrix_arma M(X.num);

    CVector_arma F0 = GetResiduals(X);
    for (int i=0; i < X.num; i++)
    {
        CVector_arma V = Jacobian(X, F0, i);
        for (int j=0; j<X.num; j++)
            M[i][j] = V[j];
    }

  return Transpose(M);
}


CVector_arma System::Jacobian(CVector_arma &V, CVector_arma &F0, int i)
{
  double epsilon;
  epsilon = -1e-6;
  CVector_arma V1(V);
  V1[i] += epsilon;
  CVector_arma F1;
  F1 = GetResiduals(V1);
  CVector_arma grad = (F1 - F0) / epsilon;
  if (grad.norm2() == 0)
  {
    epsilon = 1e-6;
    V1 = V;
    V1[i] += epsilon;
    F1 = GetResiduals(V1);
    grad = (F1 - F0) / epsilon;
  }
  return grad;

}

bool System::SetProp(const string &s, const double &val)
{
    if (s=="cn_weight")
    {   SolverSettings.C_N_weight = val; return true;}
    if (s=="nr_tolerance")
    {   SolverSettings.NRtolerance = val; return true;}
    if (s=="nr_coeff_reduction_factor")
    {   SolverSettings.NR_coeff_reduction_factor = val; return true;}
    if (s=="nr_timestep_reduction_factor")
    {   SolverSettings.NR_timestep_reduction_factor = val; return true;}
    if (s=="nr_timestep_reduction_factor_fail")
    {   SolverSettings.NR_timestep_reduction_factor_fail = val; return true;}
    if (s=="minimum_timestep")
    {   SolverSettings.minimum_timestep = val; return true;}
    if (s=="nr_niteration_lower")
    {   SolverSettings.NR_niteration_lower=int(val); return true;}
    if (s=="nr_niteration_upper")
    {   SolverSettings.NR_niteration_upper=int(val); return true;}
    if (s=="nr_niteration_max")
    {   SolverSettings.NR_niteration_max=int(val); return true;}
    if (s=="make_results_uniform")
    {   SolverSettings.makeresultsuniform = bool(val); return true;}

    if (s=="tstart")
    {   SimulationParameters.tstart = val; return true;}
    if (s=="tend")
    {   SimulationParameters.tend = val; return true;}
    if (s=="tend")
    {   SimulationParameters.dt0 = val; return true;}

    errorhandler.Append("","System","SetProp","Property '" + s + "' was not found!", 621);
    return false;
}

void System::InitiateOutputs()
{
    Outputs.AllOutputs.clear();

    for (int i=0; i<statevariables.size(); i++)
        Outputs.AllOutputs.append(CBTC(), statevariables[i].GetName());

    for (int i=0; i<controlparameters.size(); i++)
        Outputs.AllOutputs.append(CBTC(), controlparameters[i].GetName());

    for (int i=0; i<externalforcings.size(); i++)
        Outputs.AllOutputs.append(CBTC(), externalforcings[i].GetName());

}


void System::PopulateOutputs()
{
    for (int i=0; i<statevariables.size(); i++)
        Outputs.AllOutputs[statevariables[i].GetName()].append(SolverTempVars.t,statevariables[i].GetValue());

    for (int i=0; i<controlparameters.size(); i++)
        Outputs.AllOutputs[controlparameters[i].GetName()].append(SolverTempVars.t,controlparameters[i].GetValue());

    for (int i=0; i<externalforcings.size(); i++)
        Outputs.AllOutputs[externalforcings[i].GetName()].append(SolverTempVars.t,externalforcings[i].Object::GetValue());
}

bool System::Solve()
{
    #ifdef QT_version
    if (LogWindow())
    {
        LogWindow()->append("Simulation started!");
    }
    #else
        ShowMessage("Simulation started!");
    #endif
    SetAllParents();
    InitiateOutputs();
    PopulateOutputs();
    PrepareTimeSeries();
    SolverTempVars.dt_base = SimulationParameters.dt0;
    SolverTempVars.dt = SolverTempVars.dt_base;
    SolverTempVars.t = SimulationParameters.tstart;

    while (SolverTempVars.t<SimulationParameters.tend+SolverTempVars.dt)
    {
        SolverTempVars.dt = min(SolverTempVars.dt_base,GetMinimumNextTimeStepSize());
        if (SolverTempVars.dt<SimulationParameters.dt0/100) SolverTempVars.dt=SimulationParameters.dt0/100;
        ShowMessage(string("t = ") + numbertostring(SolverTempVars.t));
        #ifdef Debug_mode
        ShowMessage(string("t = ") + numbertostring(SolverTempVars.t) + ", dt_base = " + numbertostring(SolverTempVars.dt_base) + ", dt = " + numbertostring(SolverTempVars.dt) + ", SolverTempVars.numiterations =" + numbertostring(SolverTempVars.numiterations));
        #endif // Debug_mode
        #ifdef QT_version
        if (rtw)
        {
            updateProgress(false);
        }
        #endif

        bool success = OneStepSolve(SolverTempVars.dt);
        if (!success)
        {
            #ifdef Debug_mode
            ShowMessage("failed!");
            #endif // Debug_mode
            SolverTempVars.dt_base *= SolverSettings.NR_timestep_reduction_factor_fail;
            SolverTempVars.updatejacobian = true;
        }
        else
        {
            SolverTempVars.t += SolverTempVars.dt;
            if (SolverTempVars.numiterations>SolverSettings.NR_niteration_upper)
            {
                SolverTempVars.dt_base = max(SolverTempVars.dt*SolverSettings.NR_timestep_reduction_factor,SolverSettings.minimum_timestep);
                SolverTempVars.updatejacobian = true;
            }
            if (SolverTempVars.numiterations<SolverSettings.NR_niteration_lower)
                SolverTempVars.dt_base = min(SolverTempVars.dt_base/SolverSettings.NR_timestep_reduction_factor,SimulationParameters.dt0*10);
            ShowMessage("t = " + numbertostring(SolverTempVars.t));
            PopulateOutputs();
            Update();
            //UpdateObjectiveFunctions(SolverTempVars.t);
        }

    }

    ShowMessage("Simulation finished!");

    return true;
}

bool System::Update()
{
	bool out = true;
	for (unsigned int i = 0; i < externalforcings.size(); i++)
		externalforcings[i].UpdateValue(SolverTempVars.t);

	return out;
}

void System::ShowMessage(const string &msg) {cout<<msg<<endl;}

double System::GetMinimumNextTimeStepSize()
{
    double x=1e12;

    for (int i=0; i<externalforcings.size(); i++)
    {
        x = min(x,externalforcings[i].TimeSeries()->interpol_D(this->SolverTempVars.t));
    }
    return x;
}

void System::PrepareTimeSeries()
{
    for (int i=0; i<externalforcings.size();i++)
    {
        externalforcings[i].TimeSeries()->assign_D();
    }
}
