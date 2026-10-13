#ifndef REWARD_RULE_H
#define REWARD_RULE_H

using namespace std;

class IRewardRule
{
public:
    virtual ~IRewardRule() = default;

    virtual int Calculate(int baseScore) = 0;
};

class SimpleRewardRule : public IRewardRule
{
public:
    int Calculate(int baseScore) override;
};

#endif