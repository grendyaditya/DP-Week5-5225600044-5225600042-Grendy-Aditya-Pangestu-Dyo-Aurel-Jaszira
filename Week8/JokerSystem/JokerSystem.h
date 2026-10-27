#ifndef JOKER_SYSTEM_H
#define JOKER_SYSTEM_H

#include "../PokerHandChecker/Hand.h"
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Observer pattern: Game/run publishes an event after identifying the played hand.
struct HandPlayedEvent {
    int handNumber = 0;
    std::size_t cardCount = 0;
    HandRank rank = HandRank::HighCard;
};

class IJokerObserver {
public:
    virtual ~IJokerObserver() = default;
    virtual void onHandPlayed(const HandPlayedEvent& event) = 0;
    virtual std::string name() const = 0;
};

class JokerSubject {
public:
    void addObserver(IJokerObserver* observer);
    void notifyHandPlayed(const HandPlayedEvent& event) const;
private:
    std::vector<IJokerObserver*> observers_;
};

// Observer A: records every hand-played event.
class CountingJoker final : public IJokerObserver {
public:
    void onHandPlayed(const HandPlayedEvent& event) override;
    std::string name() const override { return "Counting Joker"; }
    int handsPlayed() const { return handsPlayed_; }
private:
    int handsPlayed_ = 0;
};

// Observer B: remembers whether the latest played hand was a Flush.
class FlushWatcherJoker final : public IJokerObserver {
public:
    void onHandPlayed(const HandPlayedEvent& event) override;
    std::string name() const override { return "Flush Watcher"; }
    bool bonusActive() const { return bonusActive_; }
private:
    bool bonusActive_ = false;
};

// Decorator pattern: the base score and score modifiers share one interface.
class IScoreComponent {
public:
    virtual ~IScoreComponent() = default;
    virtual int getScore() const = 0;
};

class BaseScore final : public IScoreComponent {
public:
    explicit BaseScore(int score) : score_(score) {}
    int getScore() const override { return score_; }
private:
    int score_;
};

class ScoreDecorator : public IScoreComponent {
public:
    explicit ScoreDecorator(std::unique_ptr<IScoreComponent> component)
        : component_(std::move(component)) {}
protected:
    std::unique_ptr<IScoreComponent> component_;
};

class BonusJokerDecorator final : public ScoreDecorator {
public:
    BonusJokerDecorator(std::unique_ptr<IScoreComponent> component, int bonus)
        : ScoreDecorator(std::move(component)), bonus_(bonus) {}
    int getScore() const override { return component_->getScore() + bonus_; }
private:
    int bonus_;
};

class MultiplierJokerDecorator final : public ScoreDecorator {
public:
    MultiplierJokerDecorator(std::unique_ptr<IScoreComponent> component, int multiplier)
        : ScoreDecorator(std::move(component)), multiplier_(multiplier) {}
    int getScore() const override { return component_->getScore() * multiplier_; }
private:
    int multiplier_;
};

#endif
