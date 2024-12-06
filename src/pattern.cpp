#include "pattern.h"
#include <cassert>

void Pattern::load(std::istream& is) {
    n_linhas = 0;
    m_colunas = 0;
    m_cells.clear();

    is >> n_linhas >> m_colunas;
    std::cout << "path.width" << m_colunas << n_linhas <<"\n";
    if (m_colunas <= 0 || n_linhas <= 0) {
        throw std::runtime_error(
            "Pattern parsing failed when reading WIDTH and HEIGHT.");
    }

    int size = 0;
    char c;
    while (is.get(c)) {
        if (c == '.' || c == '0') {
            if (size < m_colunas * n_linhas) {
                ++size;
                m_cells.push_back(CellState::Dead);
            } else {
                throw std::runtime_error("Pattern parsing failed when parsing "
                                         "cells (too many characters).");
            }
        } else if (c == 'X' || c == '1') {
            if (size < m_colunas * n_linhas) {
                ++size;
                m_cells.push_back(CellState::Alive);
            } else {
                throw std::runtime_error("Pattern parsing failed when parsing "
                                         "cells (too many characters).");
            }
        } else if (c == '?') {
            if (size < m_colunas * n_linhas) {
                ++size;
                m_cells.push_back(CellState::Unknown);
            } else {
                throw std::runtime_error("Pattern parsing failed when parsing "
                                         "cells (too many characters).");
            }
        }
    }

    if (size != m_colunas * n_linhas) {
        throw std::runtime_error("Pattern parsing failed when parsing cell "
                                 "(not enough characters).");
    }
}

const Pattern::CellState& Pattern::operator()(int x, int y) const {
    assert(x >= 0 && x < width() && y >= 0 && y < height());

    return m_cells[x + width() * y];
}
