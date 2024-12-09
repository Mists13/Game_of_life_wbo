#pragma once

#include <iostream>
#include <vector>

class InitialState {
  public:
    enum class CellState { Alive, Dead };

    void load(std::istream& is);

    bool isEmpty() const { return m_colunas == 0 || n_linhas == 0; }
    int width() const { return m_colunas; }
    int height() const { return n_linhas; }
    const CellState& operator()(int x, int y) const;

  private:
    int m_colunas = 0;
    int n_linhas = 0;
    std::vector<CellState> m_cells;
};
