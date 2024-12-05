#pragma once

#include <iostream>
#include <vector>

class Pattern {
  public:
    enum class CellState { Alive, Dead, Unknown };

    void load(std::istream& is);

    bool isEmpty() const { return m_width == 0 || m_height == 0; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    const CellState& operator()(int x, int y) const;

  private:
    int m_width = 0;
    int m_height = 0;
    std::vector<CellState> m_cells;
};
