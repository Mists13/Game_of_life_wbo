#include "futureState.h"
#include "satSolver.h"

// Contador global para rastrear variáveis únicas
static int variableCounter = 0;

FutureState::FutureState(int width, int height)
    : m_colunas(width), n_linhas(height), m_literals(width * height) {
    // Cria uma variável única para "falso"
    m_false = Minisat::mkLit(variableCounter++, true); // Incrementa o contador para gerar a variável
    addClause({~m_false});
    
    // Inicializa os literais para o campo
    for (auto& lit : m_literals) {
        lit = Minisat::mkLit(variableCounter++); // Cria uma nova variável única para cada célula
    }
}
const Minisat::Lit& FutureState::operator()(int x, int y) const {
    if (x < 0 || x >= width() || y < 0 || y >= height()) {
        return m_false;
    }
    return m_literals[x + y * m_colunas];
}

