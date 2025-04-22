#ifndef FICHEIROS_H
#define FICHEIROS_H

/* utilitários de gestão de memória */
void libertaMemoria(char **tab, int linhas);

/* IO de tabuleiros */
char **criarTabuleiro(int linhas, int colunas);
char **lerTabuleiroFicheiro(const char *nome, int *linhas, int *colunas);
int    gravarTabuleiroFicheiro(const char *nome, char **tab,
                               int linhas, int colunas);

#endif /* FICHEIROS_H */
