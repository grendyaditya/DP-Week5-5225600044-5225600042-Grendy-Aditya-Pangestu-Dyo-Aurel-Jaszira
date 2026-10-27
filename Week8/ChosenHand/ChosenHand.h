#ifndef CHOSEN_HAND_H
#define CHOSEN_HAND_H

#include "../PokerHandChecker/Hand.h"
#include <vector>
#include <stdexcept>

// Represents the cards selected by the player for one scoring attempt.
class ChosenHand {
public:
    ChosenHand() = default;
    explicit ChosenHand(const Hand& hand) : cards_(hand.cards.begin(), hand.cards.end()) {}
    explicit ChosenHand(std::vector<Card> cards) : cards_(std::move(cards)) {}

    bool addCard(const Card& card) {
        if (cards_.size() >= Hand::CardCount) return false;
        cards_.push_back(card);
        return true;
    }
    bool removeCard(std::size_t index) {
        if (index >= cards_.size()) return false;
        cards_.erase(cards_.begin() + static_cast<std::ptrdiff_t>(index));
        return true;
    }
    void clear() { cards_.clear(); }
    bool isComplete() const { return cards_.size() == Hand::CardCount; }
    std::size_t size() const { return cards_.size(); }
    const std::vector<Card>& cards() const { return cards_; }
    Hand toHand() const {
        if (!isComplete()) throw std::logic_error("ChosenHand must contain exactly five cards to score.");
        Hand hand;
        for (std::size_t i = 0; i < Hand::CardCount; ++i) hand.cards[i] = cards_[i];
        return hand;
    }
private:
    std::vector<Card> cards_;
};
#endif
