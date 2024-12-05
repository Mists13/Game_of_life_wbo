#pragma once

#include "satSolver.h"
#include <iostream>
#include <vector>

class Field {
  public:
    Field(int width, int height);

    int width() const { return m_width; }
    int height() const { return m_height; }

    const Minisat::Lit& operator()(int x, int y) const;

  private:
    int m_width = 0;
    int m_height = 0;
    std::vector<Minisat::Lit> m_literals;
    Minisat::Lit m_false;
};
