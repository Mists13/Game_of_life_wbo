#!/bin/bash

# Exit on any error
set -e


# Especifique o arquivo de entrada
input_file="patterns/123.txt"

# Compilar codigo principal (game of life)
make clean
make

# Executar script para gerar arquivo cnf
./bin/gol-sat "$input_file"

# Compilar open-wbo
cd open-wbo
make clean
make

# Executar open-wbo passando arquivo .cnf gerado na etapa anterior
./open-wbo ../output.cnf > ../wbo_solution.txt
