#include "InputGenerator.h"

#include <random>

using namespace std;

TurnInput FixedInputGenerator::Generate()
{
    return {3};
}

TurnInput RandomInputGenerator::Generate()
{
    static random_device randomDevice;
    static mt19937 generator(randomDevice());

    uniform_int_distribution<int> distribution(1, 10);

    return {distribution(generator)};
}