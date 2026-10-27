#ifndef POKER_HAND_CHECKER_H
#define POKER_HAND_CHECKER_H

#include "Hand.h"

class PokerHandChecker
{
public:
    virtual ~PokerHandChecker() = default;

    // Checks this hand type; if it does not match, forwards to the next checker.
    virtual HandRank check(const Hand& hand) = 0;
    void setNext(PokerHandChecker* next);

protected:
    HandRank checkRank(const Hand& hand, HandRank expectedRank, const char* handName);

private:
    PokerHandChecker* nextChecker_ = nullptr;
};

class HighCardChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class PairChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class TwoPairChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class ThreeOfAKindChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class StraightChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class FlushChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class FullHouseChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class FourOfAKindChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class StraightFlushChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class RoyalFlushChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class FiveOfAKindChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class FlushHouseChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };
class FlushFiveChecker final : public PokerHandChecker { public: HandRank check(const Hand& hand) override; };

#endif
