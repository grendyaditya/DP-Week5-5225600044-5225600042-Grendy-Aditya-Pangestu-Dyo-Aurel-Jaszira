#include "../ChosenHand/ChosenHand.h"
#include "../JokerSystem/JokerSystem.h"
#include <cassert>
#include <iostream>

int main() {
    Hand h; h.cards = {{Card{2,"Hearts"}, Card{3,"Spades"}, Card{4,"Clubs"}, Card{5,"Diamonds"}, Card{6,"Hearts"}}};
    ChosenHand chosen(h);
    assert(chosen.size() == 5 && chosen.isComplete());
    assert(!chosen.addCard(Card{7,"Hearts"}));
    assert(chosen.removeCard(0) && chosen.size() == 4);
    assert(chosen.addCard(Card{7,"Hearts"}) && chosen.isComplete());

    JokerSubject subject; CountingJoker observer; subject.addObserver(&observer);
    subject.notifyHandPlayed(HandPlayedEvent{1, 5});
    subject.notifyHandPlayed(HandPlayedEvent{2, 5});
    assert(observer.handsPlayed() == 2);

    FlushWatcherJoker flushWatcher;
    JokerSubject flushSubject;
    flushSubject.addObserver(&flushWatcher);
    flushSubject.notifyHandPlayed(HandPlayedEvent{1, 5, HandRank::Flush});
    assert(flushWatcher.bonusActive());
    flushSubject.notifyHandPlayed(HandPlayedEvent{2, 5, HandRank::Pair});
    assert(!flushWatcher.bonusActive());

    std::unique_ptr<IScoreComponent> score = std::make_unique<BaseScore>(20);
    score = std::make_unique<BonusJokerDecorator>(std::move(score), 10);
    score = std::make_unique<MultiplierJokerDecorator>(std::move(score), 2);
    assert(score->getScore() == 60);
    std::cout << "ChosenHand, Observer, and Decorator tests passed.\n";
}
