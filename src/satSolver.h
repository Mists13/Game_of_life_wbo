#pragma once

#include <core/Solver.h>
#include <simp/SimpSolver.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory> // Necessário para std::unique_ptr
#include <vector>

typedef Minisat::SimpSolver SatSolver;
typedef Minisat::vec<Minisat::Lit> Clause;

void addClause(const std::vector<Minisat::Lit>& c);
void addImpl(const std::vector<Minisat::Lit>& c,
             const Minisat::Lit& implied);
void saveAsCNF(const std::string& filename);
std::vector<int> loadSolution(const std::string& filename);

