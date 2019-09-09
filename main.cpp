#include <iostream>
#include "System.h"
#include <string>
using namespace std;

int main()
{
    System sys;
    StateVariable Prey;
    Prey.SetName("Prey");
    Prey.SetValue(1,Expression::timing::both);
    StateVariable Predator;
    Predator.SetName("Predator");
    Predator.SetValue(1,Expression::timing::both);

    Parameter k1, k2, k3, k4, k5;
    k1.SetName("k1"); k1.SetValue(0.2,Expression::timing::both); sys.AppendParameter(k1);
    k2.SetName("k2"); k2.SetValue(0.15,Expression::timing::both); sys.AppendParameter(k2);
    k3.SetName("k3"); k3.SetValue(0.05,Expression::timing::both); sys.AppendParameter(k3);
    k4.SetName("k4"); k4.SetValue(0.2,Expression::timing::both); sys.AppendParameter(k4);
    k5.SetName("k5"); k5.SetValue(0.06,Expression::timing::both); sys.AppendParameter(k5);

    ExternalForcing E1;
    E1.SetName("Growthrate");
    E1.TimeSeries()->readfile("GrowthRate.txt");
    sys.AppendState(Prey);
    sys.AppendState(Predator);
    sys.AppendExternalForcing(E1);

    Expression preyrateofchange("(k1*Prey) - (k2*Prey*Predator) - (k3*Prey) ",&sys);
    Expression predatorrateofchange("(k4*Predator*Prey*Growthrate) - (k5*Predator)",&sys);
    sys.state("Prey")->SetRateOfChange(preyrateofchange);
    sys.state("Predator")->SetRateOfChange(predatorrateofchange);

    cout<<preyrateofchange.calc(&sys,Expression::timing::past)<<endl;
    cout<<predatorrateofchange.calc(&sys,Expression::timing::past)<<endl;
    return 0;
}
