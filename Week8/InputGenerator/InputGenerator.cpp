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

    Hand GenerateRandomHand()
    {
        // Draw five distinct cards from a standard 52-card deck.
        std::vector<Card> deck;
        deck.reserve(52);

        for (const char* suit : Suits)
        {
            for (int value = 2; value <= 14; ++value)
                deck.push_back(Card{value, suit});
        }

        static std::random_device randomDevice;
        static std::mt19937 generator(randomDevice());
        std::shuffle(deck.begin(), deck.end(), generator);

        Hand hand;
        for (std::size_t i = 0; i < Hand::CardCount; ++i)
            hand.cards[i] = deck[i];

        return hand;
    }
}

Hand FixedInputGenerator::Generate()
{
    // A deterministic example hand: Pair of 2s.
    Hand hand;
    hand.cards = {{
        Card{2, "Hearts"}, Card{2, "Spades"}, Card{5, "Diamonds"},
        Card{8, "Clubs"}, Card{13, "Hearts"}
    }};
    return hand;
}

Hand RandomInputGenerator::Generate()
{
    return GenerateRandomHand();
}
