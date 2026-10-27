#include "../ScoringRule/ScoringRule.h"
#include <cassert>
#include <iostream>
int main() {
    SimpleScoringRule scorer;
    Hand high; high.cards = {{Card{14,"Hearts"},Card{2,"Spades"},Card{4,"Clubs"},Card{6,"Diamonds"},Card{8,"Hearts"}}};
    auto a=scorer.CalculateDetailed(high);
    assert(a.rank==HandRank::HighCard && a.chips==16 && a.mult==1 && a.finalScore()==16);
    Hand pair; pair.cards={{Card{9,"Hearts"},Card{9,"Spades"},Card{4,"Clubs"},Card{6,"Diamonds"},Card{8,"Hearts"}}};
    auto b=scorer.CalculateDetailed(pair);
    assert(b.rank==HandRank::Pair && b.chips==28 && b.mult==2 && b.finalScore()==56);
    Hand royal; royal.cards={{Card{10,"Hearts"},Card{11,"Hearts"},Card{12,"Hearts"},Card{13,"Hearts"},Card{14,"Hearts"}}};
    auto c=scorer.CalculateDetailed(royal);
    assert(c.rank==HandRank::RoyalFlush && c.chips==151 && c.mult==8 && c.finalScore()==1208);
    std::cout << "Balatro scoring tests passed.\n";
}
