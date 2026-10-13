#ifndef RUN_SESSION_H
#define RUN_SESSION_H

#include <memory>

#include "../InputGenerator/InputGenerator.h"
#include "../ScoringRule/ScoringRule.h"
#include "../RewardRule/RewardRule.h"
#include "../ShopSystem/ShopSystem.h"

using namespace std;

class RunSession
{
public:
    RunSession(
        unique_ptr<IInputGenerator> inputGenerator,
        unique_ptr<IScoringRule> scoringRule,
        unique_ptr<IRewardRule> rewardRule
    );

    void StartRun();

private:
    unique_ptr<IInputGenerator> inputGenerator_;
    unique_ptr<IScoringRule> scoringRule_;
    unique_ptr<IRewardRule> rewardRule_;

    ShopSystem shop_;

    int round_;
    int money_;
};

#endif