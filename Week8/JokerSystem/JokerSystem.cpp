#include "JokerSystem.h"
#include <algorithm>

void JokerSubject::addObserver(IJokerObserver* observer) {
    if (observer && std::find(observers_.begin(), observers_.end(), observer) == observers_.end())
        observers_.push_back(observer);
}

void JokerSubject::notifyHandPlayed(const HandPlayedEvent& event) const {
    for (IJokerObserver* observer : observers_) {
        if (observer) observer->onHandPlayed(event);
    }
}

void CountingJoker::onHandPlayed(const HandPlayedEvent&) {
    ++handsPlayed_;
}

void FlushWatcherJoker::onHandPlayed(const HandPlayedEvent& event) {
    // The effect is recalculated on each played hand, not left active accidentally.
    bonusActive_ = event.rank == HandRank::Flush || event.rank == HandRank::StraightFlush ||
                   event.rank == HandRank::RoyalFlush || event.rank == HandRank::FlushHouse ||
                   event.rank == HandRank::FlushFive;
}
