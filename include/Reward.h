#ifndef REWARD_H
#define REWARD_H

#include "Object.h"
#include "Expression.h"

class Reward : public Object
{
    public:
        Reward();
        virtual ~Reward();
        Reward(const Reward& other);
        Reward& operator=(const Reward& other);
        bool SetReward(const Expression &exp);
        double GetReward(Expression::timing tmg=Expression::timing::present)
        {
            double out = reward_value.calc(Parent(),tmg);
            return out;
        }

    protected:

    private:
        Expression reward_value;
};

#endif // REWARD_H
