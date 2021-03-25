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
    protected:

    private:
        Expression reward_value;
};

#endif // REWARD_H
