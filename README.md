# Game of life - Open wbo
GRR20185174 - Luzia Millena Santos Silva


## Compilação

# Compilar codigo principal (game of life)
make

# Compilar open-wbo
cd open-wbo
make
cd ..

## Execuções

Para gerar um estado anterior ao tabuleiro dado como entrada, basta executar o script 

`$ sh solve_game_of_life.sh`
Este será responsavel por criar um arquivo cnf com as regras do jogo e usar o solver para obter uma solução do problema.

