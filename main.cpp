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

    ControlParameter K1;
    K1.SetName("K1");
    K1.SetValue(0.2,Expression::timing::both);
    ExternalForcing E1;
    E1.SetName("E1");
    E1.TimeSeries()->readfile("inflow.txt");
    sys.AppendState(Prey);
    sys.AppendState(Predator);
    sys.AppendControlParameter(K1);
    sys.AppendExternalForcing(E1);

    Expression rateofchange("-K1*Prey*Predator",&sys);
    cout<<rateofchange.calc(&sys,Expression::timing::past)<<endl;
    return 0;
}
