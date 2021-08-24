#include "ControlParameter.h"
#include <math.h>


ControlParameter::ControlParameter(): Object::Object()
{
    const gsl_rng_type * T;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
}

ControlParameter::~ControlParameter()
{
    //dtor
}

ControlParameter::ControlParameter(const ControlParameter& other):Object::Object(other)
{
    RandomGeneratorModel = other.RandomGeneratorModel;
    randomgeneratorparameters = other.randomgeneratorparameters;
    const gsl_rng_type * T;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    limits = other.limits;
}

ControlParameter& ControlParameter::operator=(const ControlParameter& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    Object::operator=(rhs);
    RandomGeneratorModel = rhs.RandomGeneratorModel;
    randomgeneratorparameters = rhs.randomgeneratorparameters;
    limits = rhs.limits;
    const gsl_rng_type * T;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    return *this;
}

bool ControlParameter::SetValueRandom()
{
   switch(RandomGeneratorModel) {
   case randomgeneratormodel::uniform:
            SetValue(gsl_rng_uniform(r)*(limits[1]-limits[0])+limits[0],Expression::timing::present);
      break; //optional
   case randomgeneratormodel::normal:
            SetValue(gsl_ran_gaussian(r, 1)*(limits[1]-limits[0])/4.0 + 0.5*(limits[0]+limits[1]),Expression::timing::present);
      break; //optional
   case randomgeneratormodel::correlated_normal:
        double randomnoise = gsl_ran_gaussian(r, 1)*(limits[1]-limits[0])/4.0 + 0.5*(limits[0]+limits[1]);
        SetValue(GetValue(Expression::timing::past)*(1-pow(randomgeneratorparameters[0],2)) + randomgeneratorparameters[1]*randomnoise,Expression::timing::present);
   }
    return true;

}

