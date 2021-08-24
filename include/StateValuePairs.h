#ifndef STATEVALUEPAIRS_H
#define STATEVALUEPAIRS_H

#include <vector>
#include <iostream>
#include <string>
#include "dlib/mlp.h"

using namespace std;
struct _statevaluepair
{
    vector<double> state;
    double value;
};

struct _regression_parameters
{
    vector<double> coefficients;
};

struct _learning_parameters
{
    double beta;
};

enum class _trainmethod {linear_regression, ann};

class StateValuePairs
{
    public:
        StateValuePairs();
        StateValuePairs(unsigned int number_of_state_variables);
        void SetNumberofStateVariables(unsigned int number_of_state_variables);
        virtual ~StateValuePairs();
        StateValuePairs(const StateValuePairs& other);
        StateValuePairs& operator=(const StateValuePairs& other);
        bool Append(const _statevaluepair &stval)
        {
            if (statevalues.size()>0)
            {
                if (stval.state.size()!=statevalues[0].state.size())
                {
                    cout<<"The sizes of state variables does not match!"<<endl;
                    return false;
                }
            }
            statevalues.push_back(stval);

        }
        bool Append(const vector<double> &state, const double &value)
        {
            _statevaluepair stval;
            stval.state = state;
            stval.value = value;
            return Append(stval);
        }
        string GetCoefficientsAsString();
        double EstimateValue(const vector<double> &state);
        bool Train();
        bool Train_ANN();
        bool Train_MLR();
        bool InitializeANN(int n_state_variables, int n_layers, vector<int> n_hidden_nodes);
        bool Update_ANN();
        bool WriteToFile(const string &filename);
        void SetUpdateInterval(int n) {update_interval = n;}
        int UpdateInterval() {return update_interval;}
        _learning_parameters LearningParameters;
    protected:

    private:
        vector<_statevaluepair> statevalues;
        _trainmethod trainmethod = _trainmethod::linear_regression;
        _regression_parameters regression_parameters;
        int update_interval = 10;
        int train_counter = 0;

};

#endif // STATEVALUEPAIRS_H
