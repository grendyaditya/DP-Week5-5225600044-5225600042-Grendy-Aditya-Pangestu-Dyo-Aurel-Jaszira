#include "RunSession.h"

#include <iostream>

using namespace std;

RunSession::RunSession(
    unique_ptr<IInputGenerator> inputGenerator,
    unique_ptr<IScoringRule> scoringRule,
    unique_ptr<IRewardRule> rewardRule
)
    : inputGenerator_(move(inputGenerator)),
      scoringRule_(move(scoringRule)),
      rewardRule_(move(rewardRule)),
      round_(1),
      money_(0)
{
}

void RunSession::StartRun()
{
    cout << "=== RUN START ===\n\n";

    while (round_ <= 3)
    {
        cout << "Round " << round_ << "\n";

        // 1. Generate input
        TurnInput input = inputGenerator_->Generate();

        cout << "[PLAY] input generated: "
             << input.value << "\n";

        // 2. Compute base score
        int baseScore = scoringRule_->Calculate(input);

        cout << "[SCORE] base score: "
             << baseScore << "\n";

        // 3. Compute reward
        int reward = rewardRule_->Calculate(baseScore);

        cout << "[REWARD] gain: "
             << reward;

        // 4. Update money
        money_ += reward;

        cout << " | money: "
             << money_ << "\n";

        // 5. Shop phase
        shop_.ShowOffer();

        // 6. Advance round
        round_++;

        cout << "\n";
    }

    cout << "=== RUN END ===\n";
    cout << "Final money: " << money_ << "\n";
}