#!/bin/bash

# Exit on any error
set -e


# Especifique o arquivo de entrada
input_file="patterns/123.txt"

# Compilar codigo principal (game of life)
make clean
make

# Executar script para gerar arquivo cnf
./bin/game_of_life "$input_file"

# Compilar open-wbo
cd open-wbo
make clean
make

# Executar open-wbo passando arquivo .cnf gerado na etapa anterior
./open-wbo ../output.cnf > wbo_solution.txt

cd ..

# Obter as dimensoes do tabuleiro e printar o resultado do openwbo
first_line=$(head -n 1 "$input_file")
rows=$(echo "$first_line" | cut -d' ' -f1)
cols=$(echo "$first_line" | cut -d' ' -f2)

gcc -o  print_board_result print_board_result.cpp
./print_board_result wbo_solution.txt "$rows" "$cols"

