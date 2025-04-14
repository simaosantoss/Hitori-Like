#ifndef FICHEIROS_H
#define FICHEIROS_H

/* Lê de um ficheiro o número de linhas, colunas e depois o próprio tabuleiro
   Retorna ponteiro para o tabuleiro alocado. Devolve em *linhas, *colunas o tamanho. 
   Se falhar, devolve NULL e *linhas, *colunas = 0.
*/
char **lerTabuleiroPorDimensoes(const char *nomeFicheiro, int *linhas, int *colunas);

/* Grava o tabuleiro num ficheiro, com o formato:
    L C
    (L linhas com C caracteres cada)
*/
int gravarEmFicheiro(const char *nomeFicheiro, char **tabuleiro, int linhas, int colunas);

#endif
