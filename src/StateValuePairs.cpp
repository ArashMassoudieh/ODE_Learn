#include "StateValuePairs.h"
#include <gsl/gsl_multifit.h>
#include "Vector.h"


StateValuePairs::StateValuePairs()
{
    //ctor
}

StateValuePairs::~StateValuePairs()
{
    //dtor
}

StateValuePairs::StateValuePairs(const StateValuePairs& other)
{
    statevalues = other.statevalues;
}

StateValuePairs& StateValuePairs::operator=(const StateValuePairs& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    statevalues = rhs.statevalues;
    return *this;
}

bool StateValuePairs::Update_ANN()
{
    return true;
}

bool StateValuePairs::Train_ANN()
{
    return true;

}

bool StateValuePairs::InitializeANN(int n_state_variables, int n_layers, vector<int> n_hidden_nodes)
{
    return true;
}

bool StateValuePairs::Train_MLR()
{
    if (statevalues.size()==0) return false;
    double chisq;
    gsl_matrix *X, *cov;
    gsl_vector *y, *w, *c;


    X = gsl_matrix_alloc(statevalues.size(),statevalues[0].state.size()+1);
    y = gsl_vector_alloc(statevalues.size());
    w = gsl_vector_alloc(statevalues.size());
    c = gsl_vector_alloc(statevalues[0].state.size()+1);
    cov = gsl_matrix_alloc (statevalues[0].state.size()+1, statevalues[0].state.size()+1);

    for (unsigned int i=0; i<statevalues.size(); i++)
    {
        gsl_matrix_set(X, i, 0, 1.0);
        for (unsigned int j=0; j<statevalues[i].state.size(); j++)
            gsl_matrix_set(X,i,j+1,statevalues[i].state[j]);

        gsl_vector_set(y,i,statevalues[i].value);
        gsl_vector_set(w,i,1.0);

    }

    {
        gsl_multifit_linear_workspace * work = gsl_multifit_linear_alloc (statevalues.size(), statevalues[0].state.size()+1);
        gsl_multifit_wlinear (X, w, y, c, cov, &chisq, work);
        gsl_multifit_linear_free (work);
    }

    regression_parameters.coefficients.clear();
    for (unsigned int i=0; i<statevalues[0].state.size()+1; i++)
        regression_parameters.coefficients.push_back(gsl_vector_get(c,i));

    gsl_matrix_free (X);
    gsl_vector_free (y);
    gsl_vector_free (w);
    gsl_vector_free (c);
    gsl_matrix_free (cov);


}

string StateValuePairs::GetCoefficientsAsString()
{
    CVector V(regression_parameters.coefficients);
    return V.toString();

}
