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

    HighCardChecker highCardChecker_;
    PairChecker pairChecker_;
    TwoPairChecker twoPairChecker_;
    ThreeOfAKindChecker threeOfAKindChecker_;
    StraightChecker straightChecker_;
    FlushChecker flushChecker_;
    FullHouseChecker fullHouseChecker_;
    FourOfAKindChecker fourOfAKindChecker_;
    StraightFlushChecker straightFlushChecker_;
    RoyalFlushChecker royalFlushChecker_;
    FiveOfAKindChecker fiveOfAKindChecker_;
    FlushHouseChecker flushHouseChecker_;
    FlushFiveChecker flushFiveChecker_;
};

#endif
