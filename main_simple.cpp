#include <iostream>
#include "System.h"
#include <string>
using namespace std;

int main()
{
    System sys;
    StateVariable Storage;
    Storage.SetName("Storage");
    Storage.SetValue(1,Expression::timing::both);

    Parameter k1;
    cout<<1<<endl;
    k1.SetName("k1"); k1.SetValue(0.2,Expression::timing::both); sys.AppendParameter(k1);
    ExternalForcing Inflow;
    Inflow.SetName("Inflow");
    cout<<2<<endl;
    Inflow.TimeSeries()->readfile("Inflow_rate.txt");
    cout<<3<<endl;
    Reward rwd;
    rwd.SetName("Outflow");

    rwd.SetReward(Expression("k1*Storage",&sys));
    DynamicValue dyn;
    dyn.SetName("outflow");
    dyn.SetExpression(Expression("k1*Storage",&sys));
    sys.AppendState(Storage);
    sys.AppendExternalForcing(Inflow);
    sys.AppendReward(rwd);
    sys.AppendDynamicValue(dyn);
    Expression rateofchange("Inflow-k1*Storage",&sys);
    sys.state("Storage")->SetRateOfChange(rateofchange);
    cout<<rateofchange.calc(&sys,Expression::timing::past)<<endl;
    sys.SetProp("tstart",0);
    sys.SetProp("tend",10);
    sys.SetProp("dt",0.01);
    sys.Solve();
    sys.Outputs.AllOutputs.writetofile("Output.txt",true);
    return 0;
}
