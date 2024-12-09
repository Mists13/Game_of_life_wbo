#include "formula.h"
#include "futureState.h"
#include "initialState.h"
#include "satSolver.h"
#include <set>
#include <vector>
#include <algorithm> // Para std::find


void applyRules(const Minisat::Lit& cell,
          const std::vector<Minisat::Lit>& n, const Minisat::Lit& next) {
    assert(n.size() == 8);

    // Loneliness (1 vizinho vivo ou menos -> celula morre)
    for (std::size_t possiblyalive = 0; possiblyalive < n.size();
         ++possiblyalive) {
        std::vector<Minisat::Lit> cond;
        for (std::size_t dead = 0; dead < n.size(); ++dead) {
            if (dead == possiblyalive)
                continue;
            cond.push_back(~n[dead]);
        }
        addImpl(cond, ~next);
    }

    // Stagnation/preservation (2 vizinhos vivos -> celula continua viva ou morta)
    for (std::size_t alive1 = 0; alive1 < n.size(); ++alive1) {
        for (std::size_t alive2 = alive1 + 1; alive2 < n.size(); ++alive2) {
            std::vector<Minisat::Lit> cond;
            for (std::size_t i = 0; i < n.size(); ++i) {
                if (i == alive1 || i == alive2) {
                    cond.push_back(n[i]);
                } else {
                    cond.push_back(~n[i]);
                }
            }

            cond.push_back(cell);
            addImpl(cond, next);

            cond.back() = ~cell;
            addImpl(cond, ~next);
        }
    }

    // Life (3 vizinhos vivos-> celula nasce)
    for (std::size_t alive1 = 0; alive1 < n.size(); ++alive1) {
        for (std::size_t alive2 = alive1 + 1; alive2 < n.size(); ++alive2) {
            for (std::size_t alive3 = alive2 + 1; alive3 < n.size(); ++alive3) {
                std::vector<Minisat::Lit> cond;
                for (std::size_t i = 0; i < n.size(); ++i) {
                    if (i == alive1 || i == alive2 || i == alive3) {
                        cond.push_back(n[i]);
                    } else {
                        cond.push_back(~n[i]);
                    }
                }
                addImpl(cond, next);
            }
        }
    }

    // Overcrowding (Mais de 4 vizinhos vivos -> celula morre)
    for (std::size_t alive1 = 0; alive1 < n.size(); ++alive1) {
        for (std::size_t alive2 = alive1 + 1; alive2 < n.size(); ++alive2) {
            for (std::size_t alive3 = alive2 + 1; alive3 < n.size(); ++alive3) {
                for (std::size_t alive4 = alive3 + 1; alive4 < n.size();
                     ++alive4) {
                    std::vector<Minisat::Lit> cond;
                    cond.push_back(n[alive1]);
                    cond.push_back(n[alive2]);
                    cond.push_back(n[alive3]);
                    cond.push_back(n[alive4]);
                    addImpl(cond, ~next);
                }
            }
        }
    }
}

void applyGameOfLifeRules(const FutureState& current, const FutureState& next) {

    for (int x = -1; x <= current.width(); ++x) {
        for (int y = -1; y <= current.height(); ++y) {
            std::vector<Minisat::Lit> neighbours;
            for (int dx = -1; dx <= +1; ++dx) {
                for (int dy = -1; dy <= +1; ++dy) {
                    if (dx == 0 && dy == 0)
                        continue;
                    neighbours.push_back(current(x + dx, y + dy));
                }
            }

            applyRules(current(x, y), neighbours, next(x, y));
        }
    }
}

// Adiciona restricoes ao sat solver baseado no estado inicial de cada celula
void applyStateConstraints(const FutureState& futureState, const InitialState& pat) {
    assert(futureState.width() == pat.width());
    assert(futureState.height() == pat.height());

    for (int x = 0; x < futureState.width(); ++x) {
        for (int y = 0; y < futureState.height(); ++y) {
            switch (pat(x, y)) {
                case InitialState::CellState::Alive: {
                    // Cria um vetor com o literal futureState(x, y)
                    std::vector<Minisat::Lit> clause = {futureState(x, y)};
                    addClause(clause);
                    break;
                }
                case InitialState::CellState::Dead: {
                    // Cria um vetor com o literal ~futureState(x, y)
                    std::vector<Minisat::Lit> clause = {~futureState(x, y)};
                    addClause(clause);
                    break;
                }
            }
        }
    }
}
