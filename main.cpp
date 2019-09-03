#include <iostream>
#include "System.h"
#include <string>
using namespace std;

int main()
{
    System sys;
    StateVariable S1;
    S1.SetName("S1");

    ExternalForcing E1;
    E1.SetName("E1");
    E1.TimeSeries()->readfile("inflow.txt");
    sys.AppendState(S1);
    sys.AppendExternalForcing(E1);
    sys.object("S1")->SetValue(2,Expression::timing::both);
    Expression X("S1*S1",&sys);
    cout<<X.calc(&sys,Expression::timing::past)<<endl;
    return 0;
}
