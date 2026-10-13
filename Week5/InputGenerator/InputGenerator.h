#ifndef INPUT_GENERATOR_H
#define INPUT_GENERATOR_H

using namespace std;

struct TurnInput
{
    int value;
};

class IInputGenerator
{
public:
    virtual ~IInputGenerator() = default;

    virtual TurnInput Generate() = 0;
};

class FixedInputGenerator : public IInputGenerator
{
public:
    TurnInput Generate() override;
};

class RandomInputGenerator : public IInputGenerator
{
public:
    TurnInput Generate() override;
};

#endif