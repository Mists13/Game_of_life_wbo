#include <iostream>
#include <vector>
#include <algorithm> // For std::find
#include <fstream> // For std::ofstream
#include <sstream>

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


void writeBoardToFile(const std::vector<int>& solution, int n_rows, int m_columns, const std::string& filename) {
    // Open a file stream for writing
    std::ofstream outFile(filename);

    // Check if the file is open
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    for (int y = 0; y < n_rows; ++y) {
        for (int x = 0; x < m_columns; ++x) {
            // Compute the variable index (1-based)
            int varIndex = y * m_columns + x + 1;

            // Check the state of the variable in the solution
            auto it = std::find(solution.begin(), solution.end(), varIndex);
            if (it != solution.end()) {
                outFile << "x "; // Cell is alive
            } else {
                it = std::find(solution.begin(), solution.end(), -varIndex);
                if (it != solution.end()) {
                    outFile << ". "; // Cell is dead
                } else {
                    outFile << "?"; // Variable not found
                }
            }
        }
        outFile << "\n"; // Newline at the end of each row
    }

    // Close the file
    outFile.close();
}


void printBoard(const std::vector<int>& solution, int n_rows, int m_columns) {
    for (int y = 0; y < n_rows; ++y) {
        for (int x = 0; x < m_columns; ++x) {
            // Obtem a variável associada à célula

            int varIndex = y * m_columns + x + 1;

            // Check if the variable is true, false, or undefined in the solution
            // Busca o valor da variável na solução
            auto it = std::find(solution.begin(), solution.end(), varIndex);
            if (it != solution.end()) {
                std::cout << "x"; // Cell is alive
            } else {
                it = std::find(solution.begin(), solution.end(), -varIndex);
                if (it != solution.end()) {
                    std::cout << "."; // Célula morta
                } else {
                    std::cout << "?"; // Variável não encontrada
                }
            }
        }
        std::cout << "\n";
    }
}

int main(int argc, char** argv) {

    // solutionFile = "wbo_solution.cnf";
    std::string solutionFile = argv[1];

    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);

    // Carrega a solução
    std::vector<int> solution;
    try {
        solution = loadSolution(solutionFile);

        // File to write the board
        std::string filename_output = "previous_state_output.txt";

        // Write the board to a file
        writeBoardToFile(solution, rows, cols, filename_output);

        std::cout << "Board has been written to " << filename_output << "\n";

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // // std::cout << "Entrada:" << std::endl;
    // // printBoard(solution, n, m);
    // // std::cout << "Estado anterior:" << std::endl;
    // // printBoard(solution, n, m);
    // // std::cout << std::endl;

    return 0;
}
