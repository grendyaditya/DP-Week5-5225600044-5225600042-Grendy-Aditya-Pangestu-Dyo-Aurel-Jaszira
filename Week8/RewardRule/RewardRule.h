#ifndef REWARD_RULE_H
#define REWARD_RULE_H

class IRewardRule {
public:
    virtual ~IRewardRule() = default;

    // Legacy scoring-reward hook retained for compatibility with the original Week5 task.
    virtual int Calculate(int baseScore) = 0;

    // Balatro-like cash-out: money is based on blind reward and economy state,
    // never on the hand's score. Default implementation can be overridden.
    virtual int CalculateCashout(bool won, int blindReward, int handsRemaining, int money) {
        if (!won) return 0;
        const int interest = (money / 5 > 5) ? 5 : money / 5;
        return blindReward + handsRemaining + interest;
    }
};

class SimpleRewardRule : public IRewardRule {
public:
    int Calculate(int baseScore) override;
    int CalculateCashout(bool won, int blindReward, int handsRemaining, int money) override;
};

#endif
