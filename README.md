# Game of life - Open wbo
GRR20185174 - Luzia Millena Santos Silva


## Compilação

### Compilar codigo principal (game of life)
`$ make`

### Compilar open-wbo
`$ cd open-wbo make cd ..`

## Execução

Para gerar um estado anterior ao tabuleiro dado como entrada, basta executar o script 

`$ sh solve_game_of_life.sh`
Este será responsavel por criar um arquivo cnf com as regras do jogo e usar o solver para obter uma solução do problema.


## Minimização de células vivas

O open-wbo permite definir entre os algoritmos de busca:

Search algorithm (0=wbo,1=linear-su,2=msu3,3=part-msu3,4=oll,5=best)


Estado anterior obtido para uma entrada de 25x25 usando o algoritmo 5:
flag: -algorithm=5

![alt text](imgs_readme/algorithm_5.png)


Estado anterior obtido para uma entrada de 25x25 usando o algoritmo 0:
flag: -algorithm=0 

![alt text](imgs_readme/algorithm_0.png)


Estado anterior obtido para uma entrada de 25x25 usando o algoritmo 0 e uso de soft clauses para gerar um arquivo .wcnf
![alt text](imgs_readme/algorithm_0_wcnf.png)