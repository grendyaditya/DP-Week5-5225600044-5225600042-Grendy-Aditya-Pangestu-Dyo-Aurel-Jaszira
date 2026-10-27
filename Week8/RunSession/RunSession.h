#ifndef RUN_SESSION_H
#define RUN_SESSION_H
#include <memory>
#include "../InputGenerator/InputGenerator.h"
#include "../ScoringRule/ScoringRule.h"
#include "../RewardRule/RewardRule.h"
#include "../ShopSystem/ShopSystem.h"
#include "../ChosenHand/ChosenHand.h"
#include "../JokerSystem/JokerSystem.h"
#include "../BlindSystem/BlindRule.h"
class RunSession {
public:
 RunSession(std::unique_ptr<IInputGenerator>, std::unique_ptr<IScoringRule>, std::unique_ptr<IRewardRule>);
 void StartRun();
private:
 std::unique_ptr<IInputGenerator> inputGenerator_; std::unique_ptr<IScoringRule> scoringRule_;
 std::unique_ptr<IRewardRule> rewardRule_; ShopSystem shop_; JokerSubject jokerSubject_; CountingJoker countingJoker_;
 FlushWatcherJoker flushWatcherJoker_;
 BlindRule blindRule_; int money_=4; int ante_=1;
};
#endif
