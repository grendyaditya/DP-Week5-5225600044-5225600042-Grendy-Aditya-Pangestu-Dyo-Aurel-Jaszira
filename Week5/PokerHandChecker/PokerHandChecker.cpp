#include "PokerHandChecker.h"

#include <iostream>

void PokerHandChecker::setNext(PokerHandChecker* next)
{
    nextChecker_ = next;
}

HandRank PokerHandChecker::checkRank(
    const Hand& hand, HandRank expectedRank, const char* handName)
{
    if (hand.rank == expectedRank)
    {
        std::cout << handName << '\n';
        return expectedRank;
    }

    if (nextChecker_ != nullptr)
    {
        return nextChecker_->check(hand);
    }

    // Defensive fallback for an invalid/unhandled rank.
    std::cout << "High Card\n";
    return HandRank::HighCard;
}

HandRank HighCardChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::HighCard, "High Card");
}

HandRank PairChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::Pair, "Pair");
}

HandRank TwoPairChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::TwoPair, "Two Pair");
}

HandRank ThreeOfAKindChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::ThreeOfAKind, "Three of a Kind");
}

HandRank StraightChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::Straight, "Straight");
}

HandRank FlushChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::Flush, "Flush");
}

HandRank FullHouseChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::FullHouse, "Full House");
}

HandRank FourOfAKindChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::FourOfAKind, "Four of a Kind");
}

HandRank StraightFlushChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::StraightFlush, "Straight Flush");
}

HandRank RoyalFlushChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::RoyalFlush, "Royal Flush");
}

HandRank FiveOfAKindChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::FiveOfAKind, "Five of a Kind");
}

HandRank FlushHouseChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::FlushHouse, "Flush House");
}

HandRank FlushFiveChecker::check(const Hand& hand)
{
    return checkRank(hand, HandRank::FlushFive, "Flush Five");
}

