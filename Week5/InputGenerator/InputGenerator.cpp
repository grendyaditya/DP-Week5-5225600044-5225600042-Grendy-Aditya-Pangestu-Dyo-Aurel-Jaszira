#include "InputGenerator.h"

#include <algorithm>
#include <array>
#include <random>
#include <vector>

namespace
{
    constexpr std::array<const char*, 4> Suits = {
        "Spades", "Hearts", "Diamonds", "Clubs"
    };

    Hand GenerateRandomHand(HandRank rank)
    {
        // Build a standard 52-card deck and draw five distinct cards.
        std::vector<Card> deck;
        deck.reserve(52);

        for (const char* suit : Suits)
        {
            for (int value = 2; value <= 14; ++value)
            {
                deck.push_back(Card{value, suit});
            }
        }

        static std::random_device randomDevice;
        static std::mt19937 generator(randomDevice());
        std::shuffle(deck.begin(), deck.end(), generator);

        Hand hand;
        for (std::size_t i = 0; i < Hand::CardCount; ++i)
        {
            hand.cards[i] = deck[i];
        }

        // Rank is deliberately dummy: the assignment asks only to print
        // the hand type, not to evaluate the five cards.
        hand.rank = rank;
        return hand;
    }
}

Hand FixedInputGenerator::Generate()
{
    return GenerateRandomHand(HandRank::HighCard);
}

Hand RandomInputGenerator::Generate()
{
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    constexpr int handRankCount = 13;
    std::uniform_int_distribution<int> distribution(0, handRankCount - 1);

    const auto rank = static_cast<HandRank>(distribution(generator));
    return GenerateRandomHand(rank);
}
