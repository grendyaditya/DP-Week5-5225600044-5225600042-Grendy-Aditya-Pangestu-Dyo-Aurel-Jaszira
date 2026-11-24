#include "../ChosenHand/ChosenHand.h"
#include <cassert>
#include <iostream>

int main()
{
    std::vector<Card> playerHand = {
        Card{12, "Clubs"}, Card{13, "Diamonds"}, Card{3, "Diamonds"},
        Card{14, "Clubs"}, Card{11, "Hearts"}, Card{3, "Clubs"},
        Card{3, "Spades"}, Card{8, "Hearts"}
    };

    // Equivalent to the console input "367": choose positions 3, 6, and 7.
    ChosenHand chosen({playerHand[2], playerHand[5], playerHand[6]});
    assert(chosen.isValid());
    assert(chosen.size() == 3);

    Hand selected = chosen.toHand();
    assert(selected.cards.size() == 3);
    assert(selected.cards[0].value == 3);
    assert(selected.cards[1].value == 3);
    assert(selected.cards[2].value == 3);

    ChosenHand single({playerHand[0]});
    assert(single.isValid() && single.toHand().cards.size() == 1);

    ChosenHand five({playerHand[0], playerHand[1], playerHand[2], playerHand[3], playerHand[4]});
    assert(five.isValid() && five.toHand().cards.size() == 5);

    ChosenHand empty;
    assert(!empty.isValid());

    std::cout << "Selection tests passed.\n";
}
