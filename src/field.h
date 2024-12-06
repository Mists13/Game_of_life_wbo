#pragma once

#include "satSolver.h"
#include <iostream>
#include <vector>

class Field {
  public:
    Field(int width, int height);

    int width() const { return m_colunas; }
    int height() const { return n_linhas; }

    const Minisat::Lit& operator()(int x, int y) const;

  private:
    int m_colunas = 0;
    int n_linhas = 0;
    std::vector<Minisat::Lit> m_literals;
    Minisat::Lit m_false;
};
