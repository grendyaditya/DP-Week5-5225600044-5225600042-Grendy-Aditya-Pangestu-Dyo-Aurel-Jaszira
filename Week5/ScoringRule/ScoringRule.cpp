#include "ScoringRule.h"

SimpleScoringRule::SimpleScoringRule()
{
    // Strongest to weakest. The first matching checker determines the hand.
    flushFiveChecker_.setNext(&flushHouseChecker_);
    flushHouseChecker_.setNext(&fiveOfAKindChecker_);
    fiveOfAKindChecker_.setNext(&royalFlushChecker_);
    royalFlushChecker_.setNext(&straightFlushChecker_);
    straightFlushChecker_.setNext(&fourOfAKindChecker_);
    fourOfAKindChecker_.setNext(&fullHouseChecker_);
    fullHouseChecker_.setNext(&flushChecker_);
    flushChecker_.setNext(&straightChecker_);
    straightChecker_.setNext(&threeOfAKindChecker_);
    threeOfAKindChecker_.setNext(&twoPairChecker_);
    twoPairChecker_.setNext(&pairChecker_);
    pairChecker_.setNext(&highCardChecker_);
}

int SimpleScoringRule::Calculate(const Hand& hand)
{
    const HandRank rank = CheckHand(hand);
    return ConvertRankToScore(rank);
}

HandRank SimpleScoringRule::CheckHand(const Hand& hand)
{
    return flushFiveChecker_.check(hand);
}

int SimpleScoringRule::ConvertRankToScore(HandRank rank) const
{
    // Placeholder hand-type score for the existing core loop, not Balatro's full formula.
    return static_cast<int>(rank) + 1;
}
