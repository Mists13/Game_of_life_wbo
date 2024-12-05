#include "field.h"
#include "formula.h"
#include "pattern.h"
#include "satSolver.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " PATTERN_FILE\n";
        return 1;
    }

    // Armazena o nome do arquivo de entrada
    std::string patternFile = argv[1];

    // Cria uma instância da classe Options
    Pattern pat;

    std::ifstream f(patternFile);
    if (!f) {
        std::cout << "-- Error: Cannot open " << patternFile << std::endl;
        return 1;
    }
    try {
        pat.load(f);
    } catch (std::exception& e) {
        std::cout << "-- Error: " << e.what() << std::endl;
        return 1;
    }

    std::vector<Field> fields;
    
    fields.push_back(Field(pat.width(), pat.height()));
    //
    fields.push_back(Field(pat.width(), pat.height()));
    // 
    transition(fields[0], fields[1]);
    //
    patternConstraint(fields.back(), pat);
    //
    saveAsCNF("output.cnf");

    // Aguarda o arquivo de solução
    std::cout << "Aguardando o arquivo de solução gerado pelo resolvedor SAT.\n";
    std::cout << "Insira o caminho do arquivo de solução: ";
    std::string solutionFile;
    std::cin >> solutionFile;

    // Carrega a solução
    std::vector<int> solution;
    try {
        solution = loadSolution(solutionFile);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    std::cout << "Entrada:" << std::endl;
    printBoard(solution, fields[1]);
    std::cout << "Estado anterior:" << std::endl;
    printBoard(solution, fields[0]);
    std::cout << std::endl;
    
    return 0;
}
