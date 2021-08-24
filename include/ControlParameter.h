#ifndef CONTROLPARAMETER_H
#define CONTROLPARAMETER_H

#include "string.h"
#include "Object.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

enum class randomgeneratormodel {uniform, normal, correlated_normal};
class ControlParameter:public Object
{
    public:
        ControlParameter();
        virtual ~ControlParameter();
        ControlParameter(const ControlParameter& other);
        ControlParameter& operator=(const ControlParameter& other);
        bool SetValueRandom();
        bool SetLimits(const double &_minlimit, const double &_maxlimit)
        {
            if (_minlimit>_maxlimit) return false;
            limits.clear();
            limits.push_back(_minlimit);
            limits.push_back(_maxlimit);
            return true;
        }
        vector<double> randomgeneratorparameters;
        randomgeneratormodel RandomGeneratorModel;

    protected:

    private:
        vector<double> limits;
        gsl_rng * r = nullptr;

};

#endif // CONTROLPARAMETER_H
