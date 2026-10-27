#include "RunSession.h"
#include <iostream>
#include <array>
using namespace std;
RunSession::RunSession(unique_ptr<IInputGenerator> g, unique_ptr<IScoringRule> s, unique_ptr<IRewardRule> r)
 : inputGenerator_(move(g)), scoringRule_(move(s)), rewardRule_(move(r)) { jokerSubject_.addObserver(&countingJoker_); jokerSubject_.addObserver(&flushWatcherJoker_); }
void RunSession::StartRun() {
 cout << "=== BALATRO-LIKE RUN: ANTE " << ante_ << " ===\nStarting money: $" << money_ << "\n";
 const array<BlindType,3> types={BlindType::Small,BlindType::Big,BlindType::Boss};
 for (BlindType type: types) {
   BlindInfo blind=blindRule_.getBlind(ante_,type); int totalScore=0, handsRemaining=4; bool won=false;
   cout << "\n=== " << blind.name << " | Target: " << blind.target << " | Reward: $" << blind.reward << " ===\n";
   while(handsRemaining>0 && totalScore<blind.target) {
     Hand hand=inputGenerator_->Generate(); ChosenHand chosen(hand); hand=chosen.toHand();
     cout << "\nHand " << (5-handsRemaining) << " | Cards: ";
     for(size_t i=0;i<hand.cards.size();++i) cout << '[' << CardToString(hand.cards[i]) << "] ";
     cout << "\n";
     // Evaluate the poker hand first so the event carries meaningful hand-rank data.
     ScoreResult result=scoringRule_->CalculateDetailed(hand);
     jokerSubject_.notifyHandPlayed(HandPlayedEvent{countingJoker_.handsPlayed() + 1, hand.cards.size(), result.rank});

     // Decorators now participate in the live scoring path. Base score is Chips x Mult;
     // Joker modifiers alter score only and never change the player's money directly.
     const int baseScore = result.finalScore();
     std::unique_ptr<IScoreComponent> decoratedScore = std::make_unique<BaseScore>(baseScore);
     if (flushWatcherJoker_.bonusActive())
         decoratedScore = std::make_unique<BonusJokerDecorator>(std::move(decoratedScore), 10);
     if (countingJoker_.handsPlayed() > 0 && countingJoker_.handsPlayed() % 3 == 0)
         decoratedScore = std::make_unique<MultiplierJokerDecorator>(std::move(decoratedScore), 2);
     const int score = decoratedScore->getScore();
     totalScore+=score; --handsRemaining;
     cout << "Poker hand: " << HandRankToString(result.rank) << "\n";
     cout << "Base Chips: " << result.chips << " | Base Mult: " << result.mult
          << " | Base score: " << baseScore << "\n";
     if (flushWatcherJoker_.bonusActive()) cout << "Flush Watcher: +10 score\n";
     if (countingJoker_.handsPlayed() % 3 == 0) cout << "Counting Joker: x2 score (every 3 hands)\n";
     cout << "Final hand score: " << score << "\n";
     cout << "Blind progress: " << totalScore << " / " << blind.target << " | Hands left: " << handsRemaining << "\n";
   }
   won=totalScore>=blind.target;
   if(won) {
     int reward=rewardRule_->CalculateCashout(true, blind.reward, handsRemaining, money_);
     money_+=reward;
     cout << "BLIND DEFEATED! Cash-out reward: $" << reward << " | Money: $" << money_ << "\n";
   } else {
     cout << "BLIND FAILED. Score " << totalScore << " / " << blind.target << ". Run over. Money remains $" << money_ << "\n";
     return;
   }
   if(type!=BlindType::Boss) { cout << "Shop phase (display-only; no purchase inventory yet):\n"; shop_.ShowOffer(); } else cout << "Ante " << ante_ << " Boss Blind cleared.\n";
 }
 cout << "\n=== ANTE CLEARED (prototype stops after Ante 1) ===\nFinal money: $" << money_ << "\n";
}
