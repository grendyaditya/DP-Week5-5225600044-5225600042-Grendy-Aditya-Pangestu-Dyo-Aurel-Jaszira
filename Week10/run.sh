#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")"
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp InputGenerator/InputGenerator.cpp ScoringRule/ScoringRule.cpp PokerHandChecker/PokerHandChecker.cpp RewardRule/RewardRule.cpp RunSession/RunSession.cpp ShopSystem/ShopSystem.cpp JokerSystem/JokerSystem.cpp BlindSystem/BlindRule.cpp -o Task1
exec ./Task1
