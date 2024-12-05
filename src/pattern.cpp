#include "pattern.h"
#include <cassert>

void Pattern::load(std::istream& is) {
    m_width = 0;
    m_height = 0;
    m_cells.clear();

    is >> m_width >> m_height;
    if (m_width <= 0 || m_height <= 0) {
        throw std::runtime_error(
            "Pattern parsing failed when reading WIDTH and HEIGHT.");
    }

    int size = 0;
    char c;
    while (is.get(c)) {
        if (c == '.' || c == '0') {
            if (size < m_width * m_height) {
                ++size;
                m_cells.push_back(CellState::Dead);
            } else {
                throw std::runtime_error("Pattern parsing failed when parsing "
                                         "cells (too many characters).");
            }
        } else if (c == 'X' || c == '1') {
            if (size < m_width * m_height) {
                ++size;
                m_cells.push_back(CellState::Alive);
            } else {
                throw std::runtime_error("Pattern parsing failed when parsing "
                                         "cells (too many characters).");
            }
        } else if (c == '?') {
            if (size < m_width * m_height) {
                ++size;
                m_cells.push_back(CellState::Unknown);
            } else {
                throw std::runtime_error("Pattern parsing failed when parsing "
                                         "cells (too many characters).");
            }
        }
    }

    if (size != m_width * m_height) {
        throw std::runtime_error("Pattern parsing failed when parsing cell "
                                 "(not enough characters).");
    }
}

const Pattern::CellState& Pattern::operator()(int x, int y) const {
    assert(x >= 0 && x < width() && y >= 0 && y < height());

    return m_cells[x + width() * y];
}
