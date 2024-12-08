#pragma once

#include "satSolver.h"
#include <vector>
class Field;
class Pattern;

void applyGameOfLifeRules(const Field& current, const Field& next);
void patternConstraint(const Field& field, const Pattern& pat);
void printBoard(const std::vector<int>& solution, const Field& field);
