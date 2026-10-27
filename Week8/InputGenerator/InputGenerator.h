#ifndef INPUT_GENERATOR_H
#define INPUT_GENERATOR_H

#include "../PokerHandChecker/Hand.h"

class IInputGenerator
{
public:
    virtual ~IInputGenerator() = default;
    virtual Hand Generate() = 0;
};

class FixedInputGenerator : public IInputGenerator
{
public:
    Hand Generate() override;
};

class RandomInputGenerator : public IInputGenerator
{
public:
    Hand Generate() override;
};

#endif
