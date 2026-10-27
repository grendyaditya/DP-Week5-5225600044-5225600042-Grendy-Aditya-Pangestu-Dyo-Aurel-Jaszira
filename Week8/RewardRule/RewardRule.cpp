#include "RewardRule.h"

int SimpleRewardRule::Calculate(int baseScore)
{
    // Original Week5 behavior kept for the assignment's interchangeable reward rule.
    return baseScore;
}

int SimpleRewardRule::CalculateCashout(bool won, int blindReward, int handsRemaining, int money)
{
    if (!won) return 0;
    const int interest = (money / 5 > 5) ? 5 : money / 5;
    return blindReward + handsRemaining + interest;
}
