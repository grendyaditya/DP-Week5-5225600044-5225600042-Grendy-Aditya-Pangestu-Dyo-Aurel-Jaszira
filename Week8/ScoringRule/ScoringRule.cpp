#include "ScoringRule.h"
#include <algorithm>
#include <map>
#include <set>

SimpleScoringRule::SimpleScoringRule() {
    flushFiveChecker_.setNext(&flushHouseChecker_); flushHouseChecker_.setNext(&fiveOfAKindChecker_);
    fiveOfAKindChecker_.setNext(&royalFlushChecker_); royalFlushChecker_.setNext(&straightFlushChecker_);
    straightFlushChecker_.setNext(&fourOfAKindChecker_); fourOfAKindChecker_.setNext(&fullHouseChecker_);
    fullHouseChecker_.setNext(&flushChecker_); flushChecker_.setNext(&straightChecker_);
    straightChecker_.setNext(&threeOfAKindChecker_); threeOfAKindChecker_.setNext(&twoPairChecker_);
    twoPairChecker_.setNext(&pairChecker_); pairChecker_.setNext(&highCardChecker_);
}
HandRank SimpleScoringRule::CheckHand(const Hand& hand) { return flushFiveChecker_.check(hand); }
int SimpleScoringRule::Calculate(const Hand& hand) { return CalculateDetailed(hand).finalScore(); }
int CardChipValue(const Card& card) {
    if (card.value >= 11 && card.value <= 13) return 10;
    if (card.value == 14) return 11;
    return card.value;
}
std::string HandRankToString(HandRank r) {
    switch(r) {
      case HandRank::HighCard:return "High Card"; case HandRank::Pair:return "Pair";
      case HandRank::TwoPair:return "Two Pair"; case HandRank::ThreeOfAKind:return "Three of a Kind";
      case HandRank::Straight:return "Straight"; case HandRank::Flush:return "Flush";
      case HandRank::FullHouse:return "Full House"; case HandRank::FourOfAKind:return "Four of a Kind";
      case HandRank::StraightFlush:return "Straight Flush"; case HandRank::RoyalFlush:return "Royal Flush";
      case HandRank::FiveOfAKind:return "Five of a Kind"; case HandRank::FlushHouse:return "Flush House";
      case HandRank::FlushFive:return "Flush Five";
    } return "Unknown";
}
ScoreResult SimpleScoringRule::CalculateDetailed(const Hand& hand) {
    ScoreResult out; out.rank = CheckHand(hand);
    // Level 1 base Chips and Mult for Balatro's standard poker hands.
    switch(out.rank) {
      case HandRank::HighCard: out.chips=5; out.mult=1; break;
      case HandRank::Pair: out.chips=10; out.mult=2; break;
      case HandRank::TwoPair: out.chips=20; out.mult=2; break;
      case HandRank::ThreeOfAKind: out.chips=30; out.mult=3; break;
      case HandRank::Straight: out.chips=30; out.mult=4; break;
      case HandRank::Flush: out.chips=35; out.mult=4; break;
      case HandRank::FullHouse: out.chips=40; out.mult=4; break;
      case HandRank::FourOfAKind: out.chips=60; out.mult=7; break;
      case HandRank::StraightFlush: case HandRank::RoyalFlush: out.chips=100; out.mult=8; break;
      case HandRank::FiveOfAKind: out.chips=120; out.mult=12; break;
      case HandRank::FlushHouse: out.chips=140; out.mult=14; break;
      case HandRank::FlushFive: out.chips=160; out.mult=16; break;
    }
    // Which played cards score depends on the poker hand. This basic version handles
    // the standard hands; special duplicate-card hands are supported by classification.
    std::map<int,int> counts; for (const auto& c: hand.cards) ++counts[c.value];
    std::set<int> scoredValues;
    auto addRanksWithCount = [&](int n){ for(auto [v,c]:counts) if(c==n) scoredValues.insert(v); };
    switch(out.rank) {
      case HandRank::HighCard: {
        int high=0; for(const auto& c:hand.cards) high=std::max(high,c.value); scoredValues.insert(high); break;
      }
      case HandRank::Pair: addRanksWithCount(2); break;
      case HandRank::TwoPair: addRanksWithCount(2); break;
      case HandRank::ThreeOfAKind: addRanksWithCount(3); break;
      case HandRank::FourOfAKind: addRanksWithCount(4); break;
      case HandRank::FullHouse: case HandRank::Straight: case HandRank::Flush:
      case HandRank::StraightFlush: case HandRank::RoyalFlush:
      case HandRank::FiveOfAKind: case HandRank::FlushHouse: case HandRank::FlushFive:
        for(const auto& c:hand.cards) scoredValues.insert(c.value); break;
    }
    // Add card chips for all cards whose rank contributes; for repeated ranks, all matching cards count.
    for(const auto& c:hand.cards) if(scoredValues.count(c.value)) out.cardChips += CardChipValue(c);
    out.chips += out.cardChips;
    return out;
}
