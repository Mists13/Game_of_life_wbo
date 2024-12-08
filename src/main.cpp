#include "field.h"
#include "formula.h"
#include "pattern.h"
#include "satSolver.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <thread>
#include <algorithm> // std::find
#include <chrono>

void printBoard(const std::vector<int>& solution, const Field& field) {
    for (int y = 0; y < field.height(); ++y) {
        for (int x = 0; x < field.width(); ++x) {
            // Obtem a variável associada à célula (1-based index no CNF)
            int varIndex = Minisat::var(field(x, y)) + 1;

            // Busca o valor da variável na solução
            auto it = std::find(solution.begin(), solution.end(), varIndex);
            if (it != solution.end()) {
                std::cout << "X "; // Célula viva
            } else {
                it = std::find(solution.begin(), solution.end(), -varIndex);
                if (it != solution.end()) {
                    std::cout << ". "; // Célula morta
                }
            }
        }
        std::cout << "\n";
    }
}

void writeBoardToFile(const std::vector<int>& solution, int n_rows, int m_columns, const std::string& filename) {
    std::ofstream outFile(filename);

    // Verifica se o arquivo esta aberto
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    for (int y = 0; y < n_rows; ++y) {
        for (int x = 0; x < m_columns; ++x) {
            // Obtem o indice da variavel 
            int varIndex = y * m_columns + x + 2;

            // Verifica o estado da variavel na solucao
            auto it = std::find(solution.begin(), solution.end(), varIndex);
            if (it != solution.end()) {
                outFile << "x "; // Celula esta viva
            } else {
                it = std::find(solution.begin(), solution.end(), -varIndex);
                if (it != solution.end()) {
                    outFile << ". "; // Celula esta morta
                }
            }
        }
        outFile << "\n";
    }

    // Fecha o arquivo
    outFile.close();
}

void processSolutionFile(const std::string &solutionFile) {
    // Wait for the file to be fully written
    while (!std::filesystem::exists(solutionFile)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Check every 100ms
    }

    // Check if the file size increases over time, indicating it's still being written
    std::filesystem::path filePath(solutionFile);
    auto fileSize = std::filesystem::file_size(filePath);

    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait for 500ms before checking file size again
    auto newFileSize = std::filesystem::file_size(filePath);

    if (fileSize == newFileSize) {
        std::cout << "File is fully written\n";
    } else {
        std::cerr << "The solution file is still being written. Please wait and try again." << std::endl;
    }
}

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " PATTERN_FILE\n";
        return 1;
    }

    // Armazena o nome do arquivo de entrada
    std::string matriz_entrada = argv[1];

    // Cria uma instância de tabuleiro
    Pattern board;

    std::ifstream f(matriz_entrada);
    if (!f) {
        std::cout << "-- Error: Cannot open " << matriz_entrada << std::endl;
        return 1;
    }
    
    board.load(f);
    
    std::vector<Field> fields;

    int n = board.height();
    int m = board.width();

    fields.push_back(Field(m, n));
    fields.push_back(Field(m, n));

    applyGameOfLifeRules(fields[0], fields[1]);
    
    patternConstraint(fields.back(), board);
    
    saveAsCNF("output.cnf");
    std::cout << "Gerado arquivo output.cnf\n";

    std::string command = "./open-wbo/open-wbo -algorithm=0 ./output.cnf > ./wbo_solution.txt";
    int result = std::system(command.c_str());

    std::cout << "SAT solver executed successfully." << std::endl;
    processSolutionFile("./wbo_solution.txt");    
    
    // Aguarda o arquivo de solução gerado pelo resolvedor SAT
    std::string solutionFile = "./wbo_solution.txt";
    while (!std::filesystem::exists(solutionFile)) {
        // Sleep for 0.1 seconds
        std::cout << "Aguardando arquivo wbo_solution";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Arquivo de solucao " << solutionFile << " foi criado!" << std::endl;

    std::vector<int> solution;
    solution = loadSolution(solutionFile);

    // Arquivo para salvar solucao formatada
    std::string filename_output = "previous_state_output.txt";

    writeBoardToFile(solution, n, m, filename_output);

    std::cout << "Tabueiro resultante foi salvo em " << filename_output << "\n";

    std::cout << "Entrada:" << std::endl;
    printBoard(solution, fields[1]);
    std::cout << "Estado anterior:" << std::endl;
    printBoard(solution, fields[0]);
    std::cout << std::endl;
    
    return 0;
}
