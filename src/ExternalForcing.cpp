#include "ExternalForcing.h"

ExternalForcing::ExternalForcing(): Object::Object()
{
    //ctor
}

ExternalForcing::~ExternalForcing()
{
    //dtor
}

ExternalForcing::ExternalForcing(const ExternalForcing& other):Object::Object(other)
{
    timeseries = other.timeseries;
}

ExternalForcing& ExternalForcing::operator=(const ExternalForcing& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    Object::operator=(rhs);
    timeseries = rhs.timeseries;
    return *this;
}

double ExternalForcing::GetValue(double t)
{
    return timeseries.interpol(t);
}
void ExternalForcing::UpdateValue(double t)
{
    SetValue(Object::GetValue(Expression::timing::present),Expression::timing::past);
    SetValue(GetValue(t),Expression::timing::present);
}
