#include "ScoringRule.h"

int SimpleScoringRule::Calculate(const TurnInput& input)
{
    return input.value;
}