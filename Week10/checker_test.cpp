#include <iostream>
#include <string>
#include <vector>

#include "ScoringRule/ScoringRule.h"

namespace
{
    Hand MakeHand(std::initializer_list<Card> cards)
    {
        Hand hand;
        hand.cards = cards;
        return hand;
    }

    struct TestCase
    {
        const char* name;
        Hand hand;
        HandRank expected;
    };
}

int main()
{
    SimpleScoringRule scoringRule;
    const std::vector<TestCase> cases = {
        {"High Card", MakeHand({Card{2,"Hearts"}, Card{5,"Spades"}, Card{7,"Diamonds"}, Card{9,"Clubs"}, Card{13,"Hearts"}}), HandRank::HighCard},
        {"Pair", MakeHand({Card{2,"Hearts"}, Card{2,"Spades"}, Card{5,"Diamonds"}, Card{8,"Clubs"}, Card{13,"Hearts"}}), HandRank::Pair},
        {"Two Pair", MakeHand({Card{2,"Hearts"}, Card{2,"Spades"}, Card{5,"Diamonds"}, Card{5,"Clubs"}, Card{13,"Hearts"}}), HandRank::TwoPair},
        {"Three of a Kind", MakeHand({Card{2,"Hearts"}, Card{2,"Spades"}, Card{2,"Diamonds"}, Card{8,"Clubs"}, Card{13,"Hearts"}}), HandRank::ThreeOfAKind},
        {"Straight", MakeHand({Card{2,"Hearts"}, Card{3,"Spades"}, Card{4,"Diamonds"}, Card{5,"Clubs"}, Card{6,"Hearts"}}), HandRank::Straight},
        {"Flush", MakeHand({Card{2,"Hearts"}, Card{5,"Hearts"}, Card{7,"Hearts"}, Card{9,"Hearts"}, Card{13,"Hearts"}}), HandRank::Flush},
        {"Full House", MakeHand({Card{2,"Hearts"}, Card{2,"Spades"}, Card{2,"Diamonds"}, Card{5,"Clubs"}, Card{5,"Hearts"}}), HandRank::FullHouse},
        {"Four of a Kind", MakeHand({Card{2,"Hearts"}, Card{2,"Spades"}, Card{2,"Diamonds"}, Card{2,"Clubs"}, Card{13,"Hearts"}}), HandRank::FourOfAKind},
        {"Straight Flush", MakeHand({Card{2,"Hearts"}, Card{3,"Hearts"}, Card{4,"Hearts"}, Card{5,"Hearts"}, Card{6,"Hearts"}}), HandRank::StraightFlush},
        {"Royal Flush", MakeHand({Card{10,"Hearts"}, Card{11,"Hearts"}, Card{12,"Hearts"}, Card{13,"Hearts"}, Card{14,"Hearts"}}), HandRank::RoyalFlush},
        {"Five of a Kind", MakeHand({Card{7,"Hearts"}, Card{7,"Spades"}, Card{7,"Diamonds"}, Card{7,"Clubs"}, Card{7,"Hearts"}}), HandRank::FiveOfAKind},
        {"Flush House", MakeHand({Card{7,"Hearts"}, Card{7,"Hearts"}, Card{7,"Hearts"}, Card{9,"Hearts"}, Card{9,"Hearts"}}), HandRank::FlushHouse},
        {"Flush Five", MakeHand({Card{7,"Hearts"}, Card{7,"Hearts"}, Card{7,"Hearts"}, Card{7,"Hearts"}, Card{7,"Hearts"}}), HandRank::FlushFive},
        {"Three-card Pair", MakeHand({Card{8,"Hearts"}, Card{8,"Spades"}, Card{4,"Clubs"}}), HandRank::Pair},
        {"Three-card Three of a Kind", MakeHand({Card{6,"Hearts"}, Card{6,"Spades"}, Card{6,"Clubs"}}), HandRank::ThreeOfAKind},
        {"Four-card Two Pair", MakeHand({Card{4,"Hearts"}, Card{4,"Spades"}, Card{9,"Hearts"}, Card{9,"Clubs"}}), HandRank::TwoPair}
    };

    bool allPassed = true;
    std::cout << "=== Week 10 HandRank Classification Tests ===\n\n";
    for (const TestCase& test : cases)
    {
        const HandRank actual = scoringRule.CheckHand(test.hand);
        const bool passed = actual == test.expected;
        std::cout << test.name << ": " << (passed ? "PASS" : "FAIL")
                  << " (detected: " << HandRankToString(actual) << ")\n";
        allPassed = allPassed && passed;
    }

    std::cout << (allPassed ? "\nAll tests passed.\n" : "\nSome tests failed.\n");
    return allPassed ? 0 : 1;
}
