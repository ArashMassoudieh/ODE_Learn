#include "Reward.h"

Reward::Reward() : Object::Object()
{
    //ctor
}

Reward::~Reward()
{
    //dtor
}

Reward::Reward(const Reward& other):Object::Object(other)
{
    reward_value = other.reward_value;
}

Reward& Reward::operator=(const Reward& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    Object::operator=(rhs);
    reward_value=rhs.reward_value;
    return *this;
}

bool Reward::SetReward(const Expression &exp)
{
    reward_value=exp;
    return true;
}
