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

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " PATTERN_FILE\n";
        return 1;
    }

    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << "Current working directory: " << cwd << std::endl;
    } else {
        perror("getcwd error");
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

    transition(fields[0], fields[1]);
    
    patternConstraint(fields.back(), board);
    
    saveAsCNF("output.cnf");
    std::cout << "Gerado arquivo output.cnf\n";

    std::string command = "./open-wbo/open-wbo ./output.cnf > ./wbo_solution.txt";
    int result = std::system(command.c_str());
    if (WIFEXITED(result)) {
        int exitCode = WEXITSTATUS(result);
        if (exitCode != 0) {
            std::cerr << "SAT solver exited with code: " << exitCode << std::endl;
            // return 1;
        }
    } else {
        std::cerr << "SAT solver terminated abnormally!" << std::endl;
        // return 1;
    }
    
    // Aguarda o arquivo de solução gerado pelo resolvedor SAT
    std::string solutionFile = "./wbo_solution.txt";
    while (!std::filesystem::exists(solutionFile)) {
        // Sleep for 0.1 seconds
        std::cout << "Aguardando";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Arquivo de solucao " << solutionFile << " foi criado!" << std::endl;

    std::vector<int> solution;
    solution = loadSolution(solutionFile);

    // Arquivo para salvar solucao formatada
    std::string filename_output = "previous_state_output.txt";

    writeBoardToFile(solution, n, m, filename_output);

    std::cout << "Tabueiro resultante foi salvo em " << filename_output << "\n";

    // std::cout << "Entrada:" << std::endl;
    // printBoard(solution, fields[1]);
    // std::cout << "Estado anterior:" << std::endl;
    // printBoard(solution, fields[0]);
    // std::cout << std::endl;
    
    return 0;
}
