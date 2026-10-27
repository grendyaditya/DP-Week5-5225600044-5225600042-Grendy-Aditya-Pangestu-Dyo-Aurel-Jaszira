#ifndef SCORING_RULE_H
#define SCORING_RULE_H

#include "../InputGenerator/InputGenerator.h"
#include "../PokerHandChecker/PokerHandChecker.h"
#include <string>

struct ScoreResult {
    HandRank rank = HandRank::HighCard;
    int chips = 0;
    int mult = 1;
    int cardChips = 0;
    int finalScore() const { return chips * mult; }
};

class IScoringRule {
public:
    virtual ~IScoringRule() = default;
    virtual int Calculate(const Hand& hand) = 0;
    virtual ScoreResult CalculateDetailed(const Hand& hand) = 0;
};

class SimpleScoringRule : public IScoringRule {
public:
    SimpleScoringRule();
    int Calculate(const Hand& hand) override;
    ScoreResult CalculateDetailed(const Hand& hand) override;
    HandRank CheckHand(const Hand& hand);
private:
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

std::string HandRankToString(HandRank rank);
int CardChipValue(const Card& card);
#endif
