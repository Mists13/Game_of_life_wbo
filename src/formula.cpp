#include "formula.h"
#include "field.h"
#include "pattern.h"
#include "satSolver.h"
#include <set>
#include <vector>
#include <algorithm> // Para std::find


void rule(const Minisat::Lit& cell,
          const std::vector<Minisat::Lit>& n, const Minisat::Lit& next) {
    assert(n.size() == 8);

    // Under population (<=1 alive neighbor -> cell dies)
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

    // status quo (=2 alive neighbours -> cell stays dead/alive)
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

    // Birth (= 3 alive neighbors -> cell is alive)
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

    // Over population (>= 4 alive neighbors -> cell dies)
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

void applyGameOfLifeRules(const Field& current, const Field& next) {

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

            rule(current(x, y), neighbours, next(x, y));
        }
    }
}

void patternConstraint(const Field& field, const Pattern& pat) {
    assert(field.width() == pat.width());
    assert(field.height() == pat.height());

    for (int x = 0; x < field.width(); ++x) {
        for (int y = 0; y < field.height(); ++y) {
            switch (pat(x, y)) {
            case Pattern::CellState::Alive: {
                // Cria um vetor com o literal field(x, y)
                std::vector<Minisat::Lit> clause = {field(x, y)};
                addClause(clause);
                break;
            }
            case Pattern::CellState::Dead: {
                // Cria um vetor com o literal ~field(x, y)
                std::vector<Minisat::Lit> clause = {~field(x, y)};
                addClause(clause);
                break;
            }
            case Pattern::CellState::Unknown:
                // Nenhuma restrição
                break;
            }
        }
    }
}
