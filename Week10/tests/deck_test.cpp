#include "../InputGenerator/InputGenerator.h"
#include <cassert>
#include <iostream>
#include <set>
#include <string>

int main()
{
    RandomInputGenerator generator;
    const std::vector<Card> initial = generator.Generate();
    assert(initial.size() == 8);

    std::set<std::string> uniqueInitial;
    for (const Card& card : initial)
        uniqueInitial.insert(std::to_string(card.value) + card.suit);
    assert(uniqueInitial.size() == 8);

    const std::vector<Card> replacements = generator.Draw(3);
    assert(replacements.size() == 3);

    // All cards initially dealt plus cards drawn as replacements remain distinct.
    for (const Card& card : replacements)
    {
        const std::string key = std::to_string(card.value) + card.suit;
        assert(uniqueInitial.count(key) == 0);
        uniqueInitial.insert(key);
    }
    assert(uniqueInitial.size() == 11);

    generator.Discard(replacements);
    std::cout << "Persistent deck and refill tests passed.\n";
}
