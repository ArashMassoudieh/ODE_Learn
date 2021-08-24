#ifndef SYSTEM_H
#define SYSTEM_H

#include "StateVariable.h"
#include "ControlParameter.h"
#include "ExternalForcing.h"
#include "Parameter.h"
#include "Reward.h"
#include "DynamicValue.h"
#include <vector>
#include "Vector_arma.h"
#include "BTCSet.h"
#include "ErrorHandler.h"
#define CVector_arma CVector
#define CMatrix_arma CMatrix
#include "StateValuePairs.h"


struct _RL_parameters
{
    double alpha = 0.05;
    double gamma = 0.95;
};

using namespace std;

enum class object_type {state, control, exforce, parameter, reward, dynamicvalue, not_found};

struct solversettings
{
    double C_N_weight; //Crank-Nicholson Weight
    double NRtolerance = 1e-6; //Newton Raphson Tolerance
    double NR_coeff_reduction_factor = 0.8; //The coefficient to reduce the Newton-Raphson coefficient
    double NR_timestep_reduction_factor = 0.75;
    double NR_timestep_reduction_factor_fail = 0.3;
    double minimum_timestep = 1e-7;
    int NR_niteration_lower=20;
    int NR_niteration_upper=40;
    int NR_niteration_max=100;
    bool makeresultsuniform = false;
};

struct outputs
{
    CBTCSet AllOutputs;
    CBTCSet ObservedOutputs;
};

struct solvertemporaryvars
{
    CMatrix_arma Inverse_Jacobian;
    double NR_coefficient = 1;
    bool updatejacobian = true;
    int numiterations;
    int epoch_count=0;
    string fail_reason;
    double t;
    double dt;
    double dt_base;
};

struct simulationparameters
{
    double tstart = 0; //start time of simulation
    double tend = 1; //end time of simulation
    double dt0 = 0.01; // initial time-step size
};

struct _directories
{
    string inputpath;
    string outputpath;
};


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
        DynamicValue *dynamicvalue(const string &s);
        Parameter *parameter(const string &s);
        StateVariable *state(unsigned int i);
        ControlParameter *control(unsigned int i);
        ExternalForcing *exforce(unsigned int i);
        DynamicValue *dynamicvalue(unsigned int i);
        Parameter *parameter(unsigned int i);
        unsigned int Count(object_type objtype);
        Reward *reward(const string &s);
        double GetValue(const string &param, Expression::timing tmg=Expression::timing::present);
        object_type GetType(const string &param);
        bool AppendState(const StateVariable &stt);
        bool AppendControlParameter(const ControlParameter &ctr);
        bool AppendExternalForcing(const ExternalForcing &extforce);
        bool AppendParameter(const Parameter &param);
        bool AppendReward(const Reward &rwd);
        bool AppendDynamicValue(const DynamicValue &dyn);
        bool OneStepSolve(double dt);
        double dt() {return SolverTempVars.dt;}
        bool SetProp(const string &s, const double &val);
        ErrorHandler errorhandler;
        outputs Outputs;
        bool Solve();
        void ShowMessage(const string &msg);
        _statevaluepair GetCurrentStateValuePair();
        StateValuePairs *GetStateValuePair() {return StateValues;}
        CMatrix_arma GradientvsControlParameter(const double &dt);
        double UpdateValue();
        void UpdateControlParameters();
        double EvaluateValue(Expression::timing tmg);
        double GetTotalRewards(Expression::timing tmg);
    protected:

    private:
        void InitiateOutputs();
        void PopulateOutputs();
        void PrepareTimeSeries();
        void SetAllParents();
        vector<StateVariable> statevariables;
        vector<ControlParameter> controlparameters;
        vector<ExternalForcing> externalforcings;
        vector<Parameter> parameters;
        vector<Reward> rewards;
        vector<DynamicValue> dynamicvalues;
        CVector_arma GetStateVariables(Expression::timing tmg);
        CVector_arma GetResiduals(CVector_arma &X);
        bool Renew();
        void SetStateVariables(CVector_arma &X,Expression::timing tmg = Expression::timing::present);
        solvertemporaryvars SolverTempVars;
        solversettings SolverSettings;
        simulationparameters SimulationParameters;
        CMatrix_arma Jacobian(CVector_arma &X);
        CVector_arma Jacobian(CVector_arma &V, CVector_arma &F0, int i);
        bool Update();
        double GetMinimumNextTimeStepSize();
        double new_value=0;
        double old_value=0;
        double totalreward;
        StateValuePairs *StateValues = nullptr;

};

#endif // SYSTEM_H
