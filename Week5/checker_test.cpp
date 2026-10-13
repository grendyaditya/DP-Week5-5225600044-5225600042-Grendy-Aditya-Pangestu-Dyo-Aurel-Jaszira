#include <array>
#include <iostream>

#include "PokerHandChecker/Hand.h"
#include "ScoringRule/ScoringRule.h"

int main()
{
    SimpleScoringRule scoringRule;

    constexpr std::array<HandRank, 13> testCases = {
        HandRank::HighCard,
        HandRank::Pair,
        HandRank::TwoPair,
        HandRank::ThreeOfAKind,
        HandRank::Straight,
        HandRank::Flush,
        HandRank::FullHouse,
        HandRank::FourOfAKind,
        HandRank::StraightFlush,
        HandRank::RoyalFlush,
        HandRank::FiveOfAKind,
        HandRank::FlushHouse,
        HandRank::FlushFive
    };

    std::cout << "=== Chain of Responsibility Test ===\n\n";

    for (const HandRank rank : testCases)
    {
        Hand testHand;
        testHand.rank = rank;
        scoringRule.CheckHand(testHand);
    }

    std::cout << "\n=== Test Completed ===\n";
    return 0;
}
