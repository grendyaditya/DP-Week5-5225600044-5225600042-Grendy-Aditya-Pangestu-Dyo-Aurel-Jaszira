#include "BlindRule.h"
BlindInfo BlindRule::getBlind(int ante, BlindType type) const {
    // Ante 1 matches standard targets. Later antes use a simple scaling approximation,
    // intentionally isolated here so the exact Balatro scaling can be refined separately.
    int base = type == BlindType::Small ? 100 : (type == BlindType::Big ? 200 : 250);
    int target = base;
    if (ante > 1) { for (int a=2; a<=ante; ++a) target = static_cast<int>(target * (a==2 ? 2.0 : 1.5)); }
    if (type == BlindType::Small) return {type, "Small Blind", target, 3};
    if (type == BlindType::Big) return {type, "Big Blind", target, 4};
    return {type, "Boss Blind", target, 5};
}
bool BlindRule::checkBlind(int score, const BlindInfo& blind) const { return score >= blind.target; }
int BlindRule::cashOutReward(const BlindInfo& blind, bool won, int handsRemaining, int money) const {
    if (!won) return 0;
    // Cash-out: blind reward + $1 per unused hand; simplified $1 interest per $5, capped at $5.
    int interest = money / 5; if (interest > 5) interest = 5;
    return blind.reward + handsRemaining + interest;
}
