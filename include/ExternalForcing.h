#ifndef EXTERNALFORCING_H
#define EXTERNALFORCING_H

#include "Object.h"
#include "BTC.h"

class ExternalForcing: public Object
{
    public:
        ExternalForcing();
        virtual ~ExternalForcing();
        ExternalForcing(const ExternalForcing& other);
        ExternalForcing& operator=(const ExternalForcing& other);
        double GetValue(double t);
        void UpdateValue(double t);
        CTimeSeries *TimeSeries() {return &timeseries; }
    protected:

    private:
        CTimeSeries timeseries;
};

#endif // EXTERNALFORCING_H
