#ifndef FICHEIROS_H
#define FICHEIROS_H

/* Lê de um ficheiro o número de linhas, colunas e depois o próprio tabuleiro
   Retorna ponteiro para o tabuleiro alocado. Devolve em *linhas, *colunas o tamanho. 
   Se falhar, devolve NULL e *linhas, *colunas = 0.
*/
char **lerTabuleiroFicheiro(const char *nomeFicheiro, int *linhas, int *colunas);

int gravarTabuleiroFicheiro(const char *nomeFicheiro, char **tabuleiro, int linhas, int colunas);

/* Liberta a memória alocada para o tabuleiro. Recebe o ponteiro para o tabuleiro, o número de linhas. */
void libertaMemoria(char **tabuleiro, int linhas);

#endif
