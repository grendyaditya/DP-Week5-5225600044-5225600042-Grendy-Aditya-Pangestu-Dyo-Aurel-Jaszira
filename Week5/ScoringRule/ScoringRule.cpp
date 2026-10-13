#include "ScoringRule.h"

SimpleScoringRule::SimpleScoringRule()
{
    highCardChecker_.setNext(&pairChecker_);
    pairChecker_.setNext(&twoPairChecker_);
    twoPairChecker_.setNext(&threeOfAKindChecker_);
    threeOfAKindChecker_.setNext(&straightChecker_);
    straightChecker_.setNext(&flushChecker_);
    flushChecker_.setNext(&fullHouseChecker_);
    fullHouseChecker_.setNext(&fourOfAKindChecker_);
    fourOfAKindChecker_.setNext(&straightFlushChecker_);
    straightFlushChecker_.setNext(&royalFlushChecker_);
    royalFlushChecker_.setNext(&fiveOfAKindChecker_);
    fiveOfAKindChecker_.setNext(&flushHouseChecker_);
    flushHouseChecker_.setNext(&flushFiveChecker_);
}

int SimpleScoringRule::Calculate(const Hand& hand)
{
    // The assignment uses dummy hand classifications: run the checker chain,
    // print the detected type, then convert it to a simple numeric score.
    const HandRank rank = CheckHand(hand);
    return ConvertRankToScore(rank);
}

HandRank SimpleScoringRule::CheckHand(const Hand& hand)
{
    return highCardChecker_.check(hand);
}

int SimpleScoringRule::ConvertRankToScore(HandRank rank) const
{
    // Placeholder scores only; detailed Balatro scoring is outside this task.
    return static_cast<int>(rank) + 1;
}
