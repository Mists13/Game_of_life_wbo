#!/bin/bash
input_file="patterns/123.txt"

# Obter as dimensoes do tabuleiro e printar o resultado do openwbo
first_line=$(head -n 1 "$input_file")
rows=$(echo "$first_line" | cut -d' ' -f1)
cols=$(echo "$first_line" | cut -d' ' -f2)

# cd src
g++ src/print_board_result.cpp -o board_result
./board_result wbo_solution.txt "$rows" "$cols"

