#!/bin/bash

# Exit on any error
set -e


# Especifique o arquivo de entrada
input_file="entradas/teste_fabibi.txt"

# Executar script para gerar arquivo cnf
./bin/game-of-life "$input_file"
