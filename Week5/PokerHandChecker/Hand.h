#ifndef HAND_H
#define HAND_H

#include <array>
#include <string>

// Card data is shown to the player. The checker task does not require
// deriving the hand rank from these cards, so rank remains a dummy input.
struct Card
{
    int value = 2;          // 2-14, where 11=J, 12=Q, 13=K, 14=A
    std::string suit = "Spades";
};

enum class HandRank
{
    HighCard,
    Pair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush,
    RoyalFlush,
    FiveOfAKind,
    FlushHouse,
    FlushFive
};

struct Hand
{
    static constexpr std::size_t CardCount = 5;

    std::array<Card, CardCount> cards{};
    HandRank rank = HandRank::HighCard;
};

inline std::string CardValueToString(int value)
{
    switch (value)
    {
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        case 14: return "A";
        default: return std::to_string(value);
    }
}

inline std::string CardToString(const Card& card)
{
    std::string suitSymbol;
    if (card.suit == "Hearts") suitSymbol = "♥";
    else if (card.suit == "Diamonds") suitSymbol = "♦";
    else if (card.suit == "Clubs") suitSymbol = "♣";
    else suitSymbol = "♠";

    return CardValueToString(card.value) + suitSymbol;
}

#endif
