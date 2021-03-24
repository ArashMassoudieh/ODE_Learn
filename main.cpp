#include <iostream>
#include "System.h"
#include <string>
using namespace std;

int main()
{
    cout<<1<<endl;
    System sys;
    StateVariable Prey;
    Prey.SetName("Prey");
    Prey.SetValue(1,Expression::timing::both);
    StateVariable Predator;
    Predator.SetName("Predator");
    Predator.SetValue(1,Expression::timing::both);
    cout<<2<<endl;
    Parameter k1, k2, k3, k4, k5;
    k1.SetName("k1"); k1.SetValue(0.2,Expression::timing::both); sys.AppendParameter(k1);
    k2.SetName("k2"); k2.SetValue(0.35,Expression::timing::both); sys.AppendParameter(k2);
    k3.SetName("k3"); k3.SetValue(0.05,Expression::timing::both); sys.AppendParameter(k3);
    k4.SetName("k4"); k4.SetValue(0.2,Expression::timing::both); sys.AppendParameter(k4);
    k5.SetName("k5"); k5.SetValue(0.06,Expression::timing::both); sys.AppendParameter(k5);
    cout<<3<<endl;
    ExternalForcing E1;
    E1.SetName("Growthrate");
    E1.TimeSeries()->readfile("GrowthRate.txt");
    sys.AppendState(Prey);
    sys.AppendState(Predator);
    sys.AppendExternalForcing(E1);
    cout<<4<<endl;
    Expression preyrateofchange("(k1*Prey) - (k2*Prey*Predator) - (k3*Prey) ",&sys);
    cout<<4.1<<endl;
    Expression predatorrateofchange("(k4*Predator*Prey*Growthrate) - (k5*Predator)",&sys);
    cout<<4.2<<endl;
    sys.state("Prey")->SetRateOfChange(preyrateofchange);
    cout<<4.3<<endl;
    sys.state("Predator")->SetRateOfChange(predatorrateofchange);
    cout<<5<<endl;
    cout<<preyrateofchange.calc(&sys,Expression::timing::past)<<endl;
    cout<<predatorrateofchange.calc(&sys,Expression::timing::past)<<endl;
    sys.SetProp("tstart",0);
    sys.SetProp("tend",10);
    sys.SetProp("dt",0.01);
    cout<<6<<endl;
    sys.Solve();
    sys.Outputs.AllOutputs.writetofile("Output.txt",true);
    return 0;
}
