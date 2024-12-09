#include "futureState.h"
#include "formula.h"
#include "initialState.h"
#include "satSolver.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <thread>
#include <algorithm> // std::find
#include <chrono>
#include <cstdio>  // For std::remove


void remove_tmp_files(){

    // Remover arquivos temporarios
    if (std::remove("./wbo_solution.txt") == 0) {
        std::cout << "wbo_solution.txt removed successfully.\n";
    } else {
        std::cerr << "Error removing wbo_solution.txt\n";
    }

    if (std::remove("./output.wcnf") == 0) {
        std::cout << "output.cnf removed successfully.\n";
    } else {
        std::cerr << "Error removing output.cnf\n";
    }
}

void printBoard(const std::vector<int>& solution, const FutureState& futureState) {
    for (int y = 0; y < futureState.height(); ++y) {
        for (int x = 0; x < futureState.width(); ++x) {
            // Obtem a variável associada à célula (1-based index no CNF)
            int varIndex = Minisat::var(futureState(x, y)) + 1;

            // Busca o valor da variável na solução
            auto it = std::find(solution.begin(), solution.end(), varIndex);
            if (it != solution.end()) {
                std::cout << "1 "; // Célula viva
            } else {
                it = std::find(solution.begin(), solution.end(), -varIndex);
                if (it != solution.end()) {
                    std::cout << "0 "; // Célula morta
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
                outFile << "1 "; // Celula esta viva
            } else {
                it = std::find(solution.begin(), solution.end(), -varIndex);
                if (it != solution.end()) {
                    outFile << "0 "; // Celula esta morta
                }
            }
        }
        outFile << "\n";
    }

    // Fecha o arquivo
    outFile.close();
}

void awaitFileWrite(const std::string &solutionFile) {
    // Aguarda o arquivo ser completamente escrito
    while (!std::filesystem::exists(solutionFile)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Verifica a cada 100ms
    }

    // Verifica se o arquivo aumenta conforme o tempo,
    // indicando que ainda esta sendo escrito
    // std::filesystem::path filePath(solutionFile);
    // auto fileSize = std::filesystem::file_size(filePath);

    // std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Wait for 500ms before checking file size again
    // auto newFileSize = std::filesystem::file_size(filePath);

    // if (fileSize == newFileSize) {
    //     std::cout << "File is fully written\n";
    // } else {
    //     std::cerr << "The solution file is still being written. Please wait and try again." << std::endl;
    // }
}

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " Tabuleiro de entrada\n";
        return 1;
    }

    // Armazena o nome do arquivo de entrada
    std::string matriz_entrada = argv[1];

    // Cria uma instância de tabuleiro
    InitialState initial_state;

    std::ifstream f(matriz_entrada);
    if (!f) {
        std::cout << "-- Error: Cannot open " << matriz_entrada << std::endl;
        return 1;
    }
    
    initial_state.load(f);
    
    std::vector<FutureState> states_grid;

    int n = initial_state.height();
    int m = initial_state.width();

    states_grid.push_back(FutureState(m, n));
    states_grid.push_back(FutureState(m, n));

    auto start = std::chrono::high_resolution_clock::now();  // Start time

    applyGameOfLifeRules(states_grid[0], states_grid[1]);
    applyStateConstraints(states_grid.back(), initial_state);
    saveAsWCNF("output.wcnf");

    // Executa open-wbo com o arquivo .wcnf gerado para obter solucao
    std::string command = "./open-wbo/open-wbo -algorithm=0 ./output.wcnf > ./wbo_solution.txt";
    int result = std::system(command.c_str());

    std::cout << "Open wbo executou com sucesso." << std::endl;

    // Aguarda o arquivo de solução gerado pelo resolvedor SAT
    std::string solutionFile = "./wbo_solution.txt";
    awaitFileWrite(solutionFile);    

    auto end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Tempo total de execução da busca pela solução: " << total_duration.count() << " ms\n";

    std::cout << "Arquivo de solucao " << solutionFile << " foi criado!" << std::endl;

    std::vector<int> solution;
    solution = loadSolution(solutionFile);

    // Arquivo para salvar solucao formatada
    std::string filename_output = "previous_state_output.txt";

    writeBoardToFile(solution, n, m, filename_output);

    std::cout << "Tabueiro resultante foi salvo em " << filename_output << "\n";

    std::cout << "Entrada:" << std::endl;
    printBoard(solution, states_grid[1]);
    std::cout << "Estado anterior:" << std::endl;
    printBoard(solution, states_grid[0]);
    std::cout << std::endl;
    
    remove_tmp_files();
    return 0;
}
