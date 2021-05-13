#include <iostream>
#include "System.h"
#include <string>
using namespace std;

int main()
{

    StateValuePairs statevaluedata;

    for (unsigned int i=0; i<10; i++)
    {
        for (unsigned int j=0; j<2; j++)
        {
            vector<double> x;
            double xx = i*0.1;
            double yy = j*0.5;
            x.push_back(xx);
            x.push_back(yy);
            double val = 2*xx+0.4*yy;
            statevaluedata.Append(x,val);
        }

    }

    statevaluedata.Train_MLR();
    cout<<"Coefficients:" << statevaluedata.GetCoefficientsAsString()<<endl;
    System sys;
    StateVariable Storage;
    Storage.SetName("Storage");
    Storage.SetValue(1,Expression::timing::both);

    Parameter k, k_max, u_max, penalty;
    k.SetName("k"); k.SetValue(0.2,Expression::timing::both); sys.AppendParameter(k);
    k_max.SetName("k_max"); k_max.SetValue(0.35,Expression::timing::both); sys.AppendParameter(k_max);
    u_max.SetName("u_max"); u_max.SetValue(0.05,Expression::timing::both); sys.AppendParameter(u_max);
    penalty.SetName("penalty"); penalty.SetValue(100000,Expression::timing::both); sys.AppendParameter(penalty);


    ExternalForcing q;
    q.SetName("inflow_rate");
    q.TimeSeries()->readfile("Inflow_rate.txt");
    sys.AppendState(Storage);
    sys.AppendExternalForcing(q);
    Expression reward_exp("_min(k*Storage;u_max)-(penalty*_pos(k-k_max))",&sys);
    Expression Outflow_rate("-(k*Storage)+(0.1*inflow_rate)",&sys);
    Reward rwd;
    rwd.SetReward(reward_exp);
    rwd.SetName("reward");
    sys.state("Storage")->SetRateOfChange(Outflow_rate);
    sys.AppendReward(rwd);
    cout<<Outflow_rate.calc(&sys,Expression::timing::past)<<endl;

    sys.SetProp("tstart",0);
    sys.SetProp("tend",10);
    sys.SetProp("dt",0.01);

    sys.Solve();
    sys.Outputs.AllOutputs.writetofile("Output.txt",true);
    return 0;
}
