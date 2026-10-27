#include <memory>

#include "RunSession/RunSession.h"
#include "InputGenerator/InputGenerator.h"
#include "ScoringRule/ScoringRule.h"
#include "RewardRule/RewardRule.h"

using namespace std;

int main()
{
    unique_ptr<IInputGenerator> inputGenerator =
    make_unique<RandomInputGenerator>();

    unique_ptr<IScoringRule> scoringRule =
        make_unique<SimpleScoringRule>();

    unique_ptr<IRewardRule> rewardRule =
        make_unique<SimpleRewardRule>();

    RunSession run(
        move(inputGenerator),
        move(scoringRule),
        move(rewardRule)
    );

    run.StartRun();

    return 0;
}