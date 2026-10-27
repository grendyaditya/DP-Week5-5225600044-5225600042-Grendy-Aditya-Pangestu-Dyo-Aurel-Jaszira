# Week5 — Balatro-like Core Loop + Observer and Decorator

This C++17 project keeps the original Week5 assignment foundations and extends them with a small Balatro-inspired run. It is a learning prototype, not a complete reproduction of Balatro.

## Core loop

`RunSession` orchestrates the invariant gameplay sequence:

1. Generate a random five-card hand.
2. Represent the played cards with `ChosenHand`.
3. Classify the poker hand through the Chain of Responsibility checker.
4. Calculate base Chips, Mult, and base hand score.
5. Publish a `HandPlayedEvent` to Joker observers.
6. Apply active score effects through Decorator objects.
7. Add the final hand score to the current Blind progress.
8. On success, calculate cash-out money separately from score.
9. Show the shop offer phase and continue to the next Blind.

## Design patterns in the live run

### Chain of Responsibility

`PokerHandChecker/` contains 13 hand checkers connected strongest-to-weakest. Each checker returns its rank when matched or forwards the hand to the next checker. The test suite checks all 13 classifications, including special Balatro-style hand types that require duplicate ranks.

### Observer

`JokerSubject` is the subject and keeps a list of `IJokerObserver` listeners. After the hand is classified, `RunSession` publishes the hand number, card count, and detected rank.

- `CountingJoker` counts hand-played events. On every third hand in the run, its state activates the multiplier effect used by the scoring pipeline.
- `FlushWatcherJoker` reacts to Flush-family hands and activates a bonus for that hand.

The run notifies observers; it does not hardcode each observer's internal response into the subject.

### Decorator

`BaseScore` wraps the unmodified hand score (`Chips × Mult`). `BonusJokerDecorator` adds 10 score when `FlushWatcherJoker` is active, and `MultiplierJokerDecorator` doubles the decorated score on every third hand counted by `CountingJoker`. Both decorators are used in the actual `RunSession` scoring path, not only in an isolated test.

Order matters: the bonus is wrapped first and the multiplier wraps that result, so a qualifying third-hand Flush receives ` (base score + 10) × 2`.

## Score versus money

Score is used only to reach the Blind target. Money is a separate economy value. Winning a Blind grants its configured reward, plus one dollar per unused hand and simplified interest of one dollar per five dollars held (capped at five). The scoring result is never directly added to money. The original `IRewardRule::Calculate(int)` hook is retained for assignment compatibility; `CalculateCashout(...)` handles the separate cash-out calculation.

## Current prototype scope and limitations

- Runs Ante 1's Small, Big, and Boss Blind in sequence. Targets are 300, 450, and 600.
- The Blind class has a placeholder target-scaling approximation for later Antes; the current run stops after Ante 1.
- Each play currently generates a fresh random five-card hand. It does not yet model Balatro's persistent 8-card hand, selection UI, discard pile, or up-to-three discards.
- The shop displays an offer but does not support buying or storing a Joker inventory.
- Blind-specific Boss debuffs, editions, enhancements, seals, vouchers, and exact full-game Joker trigger ordering are not implemented.
- Cash-out interest is simplified for educational scope.
- The random generator uses a standard 52-card deck without duplicate cards; Five of a Kind, Flush House, and Flush Five are covered by checker tests but will not naturally appear in a normal generated hand.

## Build and run

From this `Week5` directory:

```bash
g++ -std=c++17 main.cpp InputGenerator/InputGenerator.cpp ScoringRule/ScoringRule.cpp PokerHandChecker/PokerHandChecker.cpp RewardRule/RewardRule.cpp RunSession/RunSession.cpp ShopSystem/ShopSystem.cpp JokerSystem/JokerSystem.cpp BlindSystem/BlindRule.cpp -o Task1
./Task1
```

## Tests

Run all 13 poker hand classification cases:

```bash
g++ -std=c++17 checker_test.cpp ScoringRule/ScoringRule.cpp PokerHandChecker/PokerHandChecker.cpp -o checker_test
./checker_test
```

Run ChosenHand, Observer, and Decorator unit tests:

```bash
g++ -std=c++17 tests/pattern_test.cpp JokerSystem/JokerSystem.cpp -o pattern_test
./pattern_test
```

Run base scoring tests:

```bash
g++ -std=c++17 tests/scoring_test.cpp ScoringRule/ScoringRule.cpp PokerHandChecker/PokerHandChecker.cpp -o scoring_test
./scoring_test
```
