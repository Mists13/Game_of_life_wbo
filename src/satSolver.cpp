
#include "satSolver.h"

// Variável global para armazenar cláusulas
std::vector<std::unique_ptr<Clause>> globalClauses;

void addClause(const std::vector<Minisat::Lit>& c) {
    auto clause = std::unique_ptr<Clause>(new Clause());
    for (const auto& lit : c) {
        clause->push(lit);
    }
    globalClauses.push_back(std::move(clause));
}

void addImpl(const std::vector<Minisat::Lit>& c, const Minisat::Lit& implied) {
    auto clause = std::unique_ptr<Clause>(new Clause());
    for (const auto& lit : c) {
        clause->push(~lit);
    }
    clause->push(implied);
    globalClauses.push_back(std::move(clause));
}

void saveAsCNF(const std::string& filename) {
    std::ofstream cnfFile(filename);

    if (!cnfFile.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo " + filename);
    }

    int maxVariable = 0;
    for (const auto& clause : globalClauses) {
        for (int i = 0; i < clause->size(); ++i) {
            const auto& lit = (*clause)[i];
            int varIndex = Minisat::var(lit) + 1;
            if (varIndex > maxVariable) {
                maxVariable = varIndex;
            }
        }
    }

    cnfFile << "p cnf " << maxVariable << " " << globalClauses.size() << "\n";

    for (const auto& clause : globalClauses) {
        for (int i = 0; i < clause->size(); ++i) {
            const auto& lit = (*clause)[i];
            cnfFile << (Minisat::sign(lit) ? "-" : "") << (Minisat::var(lit) + 1) << " ";
        }
        cnfFile << "0\n";
    }

    cnfFile.close();
}

// Função para carregar a solução do arquivo
std::vector<int> loadSolution(const std::string& filename) {
    std::vector<int> solution;
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Erro ao abrir o arquivo de solução: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Verifica se a linha começa com 'v'
        if (!line.empty() && line[0] == 'v') {
            std::istringstream iss(line.substr(1)); // Remove a letra 'v'
            int var;
            while (iss >> var) {
                solution.push_back(var);
            }
            break; // Linha de interesse encontrada, não precisa continuar lendo
        }
    }

    if (solution.empty()) {
        throw std::runtime_error("Nenhuma linha com a letra 'v' foi encontrada no arquivo: " + filename);
    }

    return solution;
}


