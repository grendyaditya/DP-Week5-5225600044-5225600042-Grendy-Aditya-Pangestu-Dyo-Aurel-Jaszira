#ifndef SCORING_RULE_H
#define SCORING_RULE_H

#include "../InputGenerator/InputGenerator.h"

using namespace std;

class IScoringRule
{
public:
    virtual ~IScoringRule() = default;

    virtual int Calculate(const TurnInput& input) = 0;
};

class SimpleScoringRule : public IScoringRule
{
public:
    int Calculate(const TurnInput& input) override;
};

#endif