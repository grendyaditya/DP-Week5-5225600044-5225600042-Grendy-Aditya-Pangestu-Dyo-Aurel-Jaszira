#pragma once
#include <string>
enum class BlindType { Small, Big, Boss };
struct BlindInfo { BlindType type; std::string name; int target; int reward; };
class BlindRule {
public:
    BlindInfo getBlind(int ante, BlindType type) const;
    bool checkBlind(int accumulatedScore, const BlindInfo& blind) const;
    int cashOutReward(const BlindInfo& blind, bool won, int handsRemaining, int money) const;
};
