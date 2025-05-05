#ifndef VALIDACAO_H
#define VALIDACAO_H

#include "tabuleiro.h"
#include <stdbool.h>

/* Funções para validação do tabuleiro */

/* Verifica se todas as casas brancas estão conectadas ortogonalmente */
int casasBrancasConectadas(char **tab, int l, int c);

/* Verifica se todos os vizinhos de uma célula com '#' são casas brancas */
int vizinhosSaoBrancos(char **tab, int l, int c, Coordenadas p);

/* Verifica as restrições básicas do tabuleiro, como letras repetidas, '#' mal posicionados e minúsculas fora de lugar */
int regrasBasicas(char **tab, int l, int c, int *dup, int *hash, int *minus);

/* Verifica se o tabuleiro segue as regras básicas e retorna se o tabuleiro está correto */
int regrasBasicasOk(char **tab, int l, int c);

/* Verifica o estado do tabuleiro e imprime os erros, se houver */
int verificaEstado(char **tab, int l, int c);

#endif /* VALIDACAO_H */
