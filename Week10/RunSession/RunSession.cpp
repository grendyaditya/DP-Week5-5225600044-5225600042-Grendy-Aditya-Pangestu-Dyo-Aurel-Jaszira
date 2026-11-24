#include "RunSession.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace
{
    constexpr std::size_t PlayerHandSize = 8;
    constexpr std::size_t MaxPlayedCards = 5;

    void PrintCards(const std::vector<Card>& cards, bool showPositions)
    {
        for (std::size_t i = 0; i < cards.size(); ++i)
        {
            if (showPositions)
                std::cout << (i + 1) << ':';
            std::cout << '[' << CardToString(cards[i]) << ']';
            if (i + 1 < cards.size())
                std::cout << ' ';
        }
        std::cout << '\n';
    }

    bool ParseSelection(const std::string& input,
                        std::size_t handSize,
                        std::vector<std::size_t>& selectedPositions)
    {
        selectedPositions.clear();
        std::vector<bool> alreadySelected(handSize, false);

        for (unsigned char character : input)
        {
            if (std::isspace(character) || character == ',' || character == '-')
                continue;

            if (!std::isdigit(character))
                return false;

            const int position = character - '0';
            if (position < 1 || static_cast<std::size_t>(position) > handSize)
                return false;

            const std::size_t index = static_cast<std::size_t>(position - 1);
            if (alreadySelected[index])
                return false;

            alreadySelected[index] = true;
            selectedPositions.push_back(index);
        }

        return !selectedPositions.empty() && selectedPositions.size() <= MaxPlayedCards;
    }

    std::vector<Card> GetSelectedCards(const std::vector<Card>& playerHand,
                                       const std::vector<std::size_t>& positions)
    {
        std::vector<Card> selected;
        selected.reserve(positions.size());
        for (std::size_t position : positions)
            selected.push_back(playerHand[position]);
        return selected;
    }

    void RemoveSelectedCards(std::vector<Card>& playerHand,
                             const std::vector<std::size_t>& positions)
    {
        // Erase from highest index to lowest so earlier indices remain valid.
        std::vector<std::size_t> descending = positions;
        std::sort(descending.begin(), descending.end(), std::greater<std::size_t>());
        for (std::size_t index : descending)
            playerHand.erase(playerHand.begin() + static_cast<std::ptrdiff_t>(index));
    }
}

RunSession::RunSession(std::unique_ptr<IInputGenerator> generator,
                       std::unique_ptr<IScoringRule> scoring,
                       std::unique_ptr<IRewardRule> reward)
    : inputGenerator_(std::move(generator)),
      scoringRule_(std::move(scoring)),
      rewardRule_(std::move(reward))
{
    jokerSubject_.addObserver(&countingJoker_);
    jokerSubject_.addObserver(&flushWatcherJoker_);
}

void RunSession::StartRun()
{
    std::cout << "=== BALATRO-LIKE RUN: ANTE " << ante_ << " ===\n";
    std::cout << "Starting money: $" << money_ << "\n";

    // The deck is shuffled once by InputGenerator. The initial holding contains eight cards.
    std::vector<Card> playerHand = inputGenerator_->Generate();
    if (playerHand.size() > PlayerHandSize)
        playerHand.resize(PlayerHandSize);

    const std::array<BlindType, 3> blindTypes = {
        BlindType::Small, BlindType::Big, BlindType::Boss
    };

    for (BlindType type : blindTypes)
    {
        const BlindInfo blind = blindRule_.getBlind(ante_, type);
        int totalScore = 0;
        int handsRemaining = 4;
        bool quitRequested = false;

        std::cout << "\n=== " << blind.name << " | Target: " << blind.target
                  << " | Reward: $" << blind.reward << " ===\n";

        while (handsRemaining > 0 && totalScore < blind.target)
        {
            if (playerHand.empty())
            {
                std::cout << "No cards remain in the player's hand. Run over.\n";
                return;
            }

            std::cout << "\nHand " << (5 - handsRemaining)
                      << " | Choose 1-5 cards from your 8-card hand:\n";
            PrintCards(playerHand, true);
            std::cout << "Input card positions (example: 415 = cards 4, 1, and 5; q = quit): ";

            std::string input;
            std::vector<std::size_t> selectedPositions;
            while (true)
            {
                if (!std::getline(std::cin, input))
                {
                    std::cout << "\nInput stream closed. Ending run.\n";
                    return;
                }

                std::string normalized = input;
                for (char& character : normalized)
                    character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));

                if (normalized == "q" || normalized == "quit")
                {
                    quitRequested = true;
                    break;
                }

                if (ParseSelection(input, playerHand.size(), selectedPositions))
                    break;

                std::cout << "Invalid selection. Enter 1-5 unique positions from the displayed hand, e.g. 415: ";
            }

            if (quitRequested)
                break;

            // ChosenHand owns only the selected one-to-five cards; the other cards remain in the holding.
            ChosenHand chosenHand(GetSelectedCards(playerHand, selectedPositions));
            if (!chosenHand.isValid())
            {
                std::cout << "Chosen hand must contain between one and five cards.\n";
                continue;
            }

            const Hand playedHand = chosenHand.toHand();
            std::cout << "Chosen Hand: ";
            PrintCards(playedHand.cards, false);

            // 1) HandRank is determined by the Week 8 Chain of Responsibility checker.
            // 2) Base Chips/Mult are calculated by ScoringRule.
            const ScoreResult result = scoringRule_->CalculateDetailed(playedHand);

            // 3) Notify Joker observers only after the selected hand has a detected rank.
            const int nextHandNumber = countingJoker_.handsPlayed() + 1;
            jokerSubject_.notifyHandPlayed(
                HandPlayedEvent{nextHandNumber, playedHand.cards.size(), result.rank});

            // 4) Apply the Week 8 Decorator effects to the base score.
            const int baseScore = result.finalScore();
            std::unique_ptr<IScoreComponent> decoratedScore =
                std::make_unique<BaseScore>(baseScore);

            if (flushWatcherJoker_.bonusActive())
                decoratedScore = std::make_unique<BonusJokerDecorator>(
                    std::move(decoratedScore), 10);

            const bool countingBonusActive =
                countingJoker_.handsPlayed() > 0 && countingJoker_.handsPlayed() % 3 == 0;
            if (countingBonusActive)
                decoratedScore = std::make_unique<MultiplierJokerDecorator>(
                    std::move(decoratedScore), 2);

            const int score = decoratedScore->getScore();
            totalScore += score;
            --handsRemaining;

            std::cout << "Poker hand: " << HandRankToString(result.rank) << '\n';
            std::cout << "Base Chips: " << result.chips
                      << " | Base Mult: " << result.mult
                      << " | Base score: " << baseScore << '\n';
            if (flushWatcherJoker_.bonusActive())
                std::cout << "Flush Watcher: +10 score\n";
            if (countingBonusActive)
                std::cout << "Counting Joker: x2 score (every 3 hands)\n";

            std::cout << "Final hand score: " << score << '\n';
            std::cout << "Blind progress: " << totalScore << " / " << blind.target
                      << " | Hands left: " << handsRemaining << '\n';

            // 5) Played cards leave the holding and enter the discard pile.
            const std::vector<Card> playedCards = chosenHand.cards();
            inputGenerator_->Discard(playedCards);
            RemoveSelectedCards(playerHand, selectedPositions);

            // 6) Draw the same number of replacements from the remaining deck.
            const std::vector<Card> replacements = inputGenerator_->Draw(playedCards.size());
            playerHand.insert(playerHand.end(), replacements.begin(), replacements.end());

            std::cout << "Refill: drew " << replacements.size()
                      << " replacement card(s). Current hand size: "
                      << playerHand.size() << '\n';
            if (playerHand.size() < PlayerHandSize)
                std::cout << "Note: the draw pile is running low; fewer than eight cards are available.\n";
        }

        if (quitRequested)
        {
            std::cout << "Run stopped by player. Final money: $" << money_ << '\n';
            return;
        }

        const bool won = totalScore >= blind.target;
        if (won)
        {
            const int reward = rewardRule_->CalculateCashout(
                true, blind.reward, handsRemaining, money_);
            money_ += reward;
            std::cout << "BLIND DEFEATED! Cash-out reward: $" << reward
                      << " | Money: $" << money_ << '\n';
        }
        else
        {
            std::cout << "BLIND FAILED. Score " << totalScore << " / " << blind.target
                      << ". Run over. Money remains $" << money_ << '\n';
            return;
        }

        if (type != BlindType::Boss)
        {
            std::cout << "Shop phase (display-only; no purchase inventory yet):\n";
            shop_.ShowOffer();
        }
        else
        {
            std::cout << "Ante " << ante_ << " Boss Blind cleared.\n";
        }
    }

    std::cout << "\n=== ANTE CLEARED (prototype stops after Ante 1) ===\n";
    std::cout << "Final money: $" << money_ << '\n';
}
