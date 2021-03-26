#ifndef STATEVALUEPAIRS_H
#define STATEVALUEPAIRS_H

#include <vector>

using namespace std;
struct _statevaluepair
{
    vector<double> state;
    double value;
};

class StateValuePairs
{
    public:
        StateValuePairs();
        virtual ~StateValuePairs();
        StateValuePairs(const StateValuePairs& other);
        StateValuePairs& operator=(const StateValuePairs& other);
        void Append(const _statevaluepair &stval) {statevalues.push_back(stval);}

    protected:

    private:
        vector<_statevaluepair> statevalues;
};

#endif // STATEVALUEPAIRS_H
