#include <array>
#include <iostream>
#include <string>

#include "ScoringRule/ScoringRule.h"

namespace
{
    Hand MakeHand(std::array<Card, 5> cards)
    {
        Hand hand;
        hand.cards = std::move(cards);
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
    const std::array<TestCase, 13> cases = {{
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
        {"Flush Five", MakeHand({Card{7,"Hearts"}, Card{7,"Hearts"}, Card{7,"Hearts"}, Card{7,"Hearts"}, Card{7,"Hearts"}}), HandRank::FlushFive}
    }};

    bool allPassed = true;
    std::cout << "=== Actual Card Classification Tests ===\n\n";
    for (const TestCase& test : cases)
    {
        std::cout << test.name << " expected | detected: ";
        const HandRank actual = scoringRule.CheckHand(test.hand);
        const bool passed = actual == test.expected;
        std::cout << (passed ? "PASS" : "FAIL") << "\n\n";
        allPassed = allPassed && passed;
    }

    std::cout << (allPassed ? "All tests passed.\n" : "Some tests failed.\n");
    return allPassed ? 0 : 1;
}
