#ifndef SCORING_RULE_H
#define SCORING_RULE_H

#include "../InputGenerator/InputGenerator.h"
#include "../PokerHandChecker/PokerHandChecker.h"

class IScoringRule
{
public:
    virtual ~IScoringRule() = default;
    virtual int Calculate(const Hand& hand) = 0;
};

class SimpleScoringRule : public IScoringRule
{
public:
    SimpleScoringRule();

    int Calculate(const Hand& hand) override;

    // Exposed for focused tests; the game also uses this chain via Calculate().
    HandRank CheckHand(const Hand& hand);

private:
    int ConvertRankToScore(HandRank rank) const;

    FlushFiveChecker flushFiveChecker_;
    FlushHouseChecker flushHouseChecker_;
    FiveOfAKindChecker fiveOfAKindChecker_;
    RoyalFlushChecker royalFlushChecker_;
    StraightFlushChecker straightFlushChecker_;
    FourOfAKindChecker fourOfAKindChecker_;
    FullHouseChecker fullHouseChecker_;
    FlushChecker flushChecker_;
    StraightChecker straightChecker_;
    ThreeOfAKindChecker threeOfAKindChecker_;
    TwoPairChecker twoPairChecker_;
    PairChecker pairChecker_;
    HighCardChecker highCardChecker_;
};

#endif
