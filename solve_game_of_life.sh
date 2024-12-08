#!/bin/bash

# Exit on any error
set -e


# Especifique o arquivo de entrada
input_file="patterns/123.txt"

# Executar script para gerar arquivo cnf
./bin/game-of-life "$input_file"

# echo "teste"
# # Executar open-wbo passando arquivo .cnf gerado na etapa anterior
# ./open-wbo/open-wbo ./output.cnf > ./wbo_solution.txt