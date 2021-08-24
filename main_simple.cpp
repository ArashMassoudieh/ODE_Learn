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
    sys.AppendState(Storage);

    ControlParameter k1;
    cout<<1<<endl;
    k1.RandomGeneratorModel = randomgeneratormodel::uniform;
    k1.SetLimits(0,2);
    k1.SetName("k1"); k1.SetValue(0.2,Expression::timing::both); sys.AppendControlParameter(k1);
    ExternalForcing Inflow;
    Inflow.SetName("Inflow");
    cout<<2<<endl;
    Inflow.TimeSeries()->readfile("Inflow_rate.txt");
    cout<<Inflow.TimeSeries()->n<<endl;
    cout<<3<<endl;
    Reward rwd;
    rwd.SetName("Shortage");
    sys.AppendExternalForcing(Inflow);
    rwd.SetReward(Expression("-_pos(0.5-k1*Storage)",&sys));
    cout<<4<<endl;
    DynamicValue dyn;
    dyn.SetName("outflow");
    dyn.SetExpression(Expression("k1*Storage",&sys));
    sys.AppendReward(rwd);
    sys.AppendDynamicValue(dyn);
    Expression rateofchange("Inflow-k1*Storage",&sys);
    sys.state("Storage")->SetRateOfChange(rateofchange);
    cout<<rateofchange.calc(&sys,Expression::timing::past)<<endl;
    sys.SetProp("tstart",0);
    sys.SetProp("tend",10);
    sys.SetProp("dt",0.01);
    sys.GetStateValuePair()->LearningParameters.beta = 0.1;
    cout<<"Solving ..."<<endl;
    sys.Solve();
    sys.Outputs.AllOutputs.writetofile("Output.txt",true);
    sys.GetStateValuePair()->WriteToFile("StateValues.txt");
    sys.GetStateValuePair()->Train_MLR();
    cout<<sys.GetStateValuePair()->GetCoefficientsAsString()<<endl;
    return 0;
}
