#include "PokerHandChecker.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace
{
    struct HandFacts
    {
        std::map<int, int> valueCounts;
        std::set<int> uniqueValues;
        bool flush = false;
        bool straight = false;
        bool royal = false;
        std::array<int, 5> sortedValues{};
        std::vector<int> groupSizes;
    };

    HandFacts Analyze(const Hand& hand)
    {
        HandFacts facts;
        std::set<std::string> suits;

        for (const Card& card : hand.cards)
        {
            ++facts.valueCounts[card.value];
            facts.uniqueValues.insert(card.value);
            suits.insert(card.suit);
        }

        facts.flush = suits.size() == 1;
        for (const auto& entry : facts.valueCounts)
            facts.groupSizes.push_back(entry.second);
        std::sort(facts.groupSizes.begin(), facts.groupSizes.end(), std::greater<int>());

        if (facts.uniqueValues.size() == Hand::CardCount)
        {
            const int minValue = *facts.uniqueValues.begin();
            const int maxValue = *facts.uniqueValues.rbegin();
            facts.straight = (maxValue - minValue == 4);

            // Ace-low straight: A, 2, 3, 4, 5.
            if (facts.uniqueValues == std::set<int>{2, 3, 4, 5, 14})
                facts.straight = true;

            facts.royal = facts.uniqueValues == std::set<int>{10, 11, 12, 13, 14};
        }

        facts.sortedValues = {0, 0, 0, 0, 0};
        std::size_t index = 0;
        for (int value : facts.uniqueValues)
        {
            if (index < facts.sortedValues.size())
                facts.sortedValues[index++] = value;
        }
        return facts;
    }

    bool IsRank(const Hand& hand, HandRank rank)
    {
        const HandFacts facts = Analyze(hand);
        const bool hasGroupOfFive = !facts.groupSizes.empty() && facts.groupSizes[0] == 5;
        const bool hasGroupOfFour = !facts.groupSizes.empty() && facts.groupSizes[0] == 4;
        const bool hasThreeAndPair = facts.groupSizes.size() == 2 &&
            facts.groupSizes[0] == 3 && facts.groupSizes[1] == 2;
        const bool hasThree = !facts.groupSizes.empty() && facts.groupSizes[0] == 3;
        const int pairCount = static_cast<int>(std::count_if(
            facts.valueCounts.begin(), facts.valueCounts.end(),
            [](const auto& entry) { return entry.second == 2; }));

        switch (rank)
        {
            case HandRank::FlushFive:
                return hasGroupOfFive && facts.flush;
            case HandRank::FlushHouse:
                return hasThreeAndPair && facts.flush;
            case HandRank::FiveOfAKind:
                return hasGroupOfFive;
            case HandRank::RoyalFlush:
                return facts.royal && facts.flush;
            case HandRank::StraightFlush:
                return facts.straight && facts.flush;
            case HandRank::FourOfAKind:
                return hasGroupOfFour;
            case HandRank::FullHouse:
                return hasThreeAndPair;
            case HandRank::Flush:
                return facts.flush;
            case HandRank::Straight:
                return facts.straight;
            case HandRank::ThreeOfAKind:
                return hasThree && !hasThreeAndPair;
            case HandRank::TwoPair:
                return pairCount == 2;
            case HandRank::Pair:
                return pairCount == 1;
            case HandRank::HighCard:
                return true;
        }
        return false;
    }
}

void PokerHandChecker::setNext(PokerHandChecker* next)
{
    nextChecker_ = next;
}

HandRank PokerHandChecker::checkRank(
    const Hand& hand, HandRank expectedRank, const char* handName)
{
    if (IsRank(hand, expectedRank))
    {
        return expectedRank;
    }

    if (nextChecker_ != nullptr)
        return nextChecker_->check(hand);

    // The High Card checker is intended to be the final fallback in the chain.
    return HandRank::HighCard;
}

HandRank HighCardChecker::check(const Hand& hand) { return checkRank(hand, HandRank::HighCard, "High Card"); }
HandRank PairChecker::check(const Hand& hand) { return checkRank(hand, HandRank::Pair, "Pair"); }
HandRank TwoPairChecker::check(const Hand& hand) { return checkRank(hand, HandRank::TwoPair, "Two Pair"); }
HandRank ThreeOfAKindChecker::check(const Hand& hand) { return checkRank(hand, HandRank::ThreeOfAKind, "Three of a Kind"); }
HandRank StraightChecker::check(const Hand& hand) { return checkRank(hand, HandRank::Straight, "Straight"); }
HandRank FlushChecker::check(const Hand& hand) { return checkRank(hand, HandRank::Flush, "Flush"); }
HandRank FullHouseChecker::check(const Hand& hand) { return checkRank(hand, HandRank::FullHouse, "Full House"); }
HandRank FourOfAKindChecker::check(const Hand& hand) { return checkRank(hand, HandRank::FourOfAKind, "Four of a Kind"); }
HandRank StraightFlushChecker::check(const Hand& hand) { return checkRank(hand, HandRank::StraightFlush, "Straight Flush"); }
HandRank RoyalFlushChecker::check(const Hand& hand) { return checkRank(hand, HandRank::RoyalFlush, "Royal Flush"); }
HandRank FiveOfAKindChecker::check(const Hand& hand) { return checkRank(hand, HandRank::FiveOfAKind, "Five of a Kind"); }
HandRank FlushHouseChecker::check(const Hand& hand) { return checkRank(hand, HandRank::FlushHouse, "Flush House"); }
HandRank FlushFiveChecker::check(const Hand& hand) { return checkRank(hand, HandRank::FlushFive, "Flush Five"); }
