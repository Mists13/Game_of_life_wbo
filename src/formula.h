#pragma once

#include "satSolver.h"
#include <vector>
class FutureState;
class InitialState;

void applyGameOfLifeRules(const FutureState& current, const FutureState& next);
void applyStateConstraints(const FutureState& futureState, const InitialState& pat);
void printBoard(const std::vector<int>& solution, const FutureState& futureState);
