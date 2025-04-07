#include <stdio.h>
#include <stdlib.h>
#include "etapa1.h"

int main(void) {
    int linhas, colunas;
    printf("Insira o tabuleiro de jogo e as suas dimensoes:\n");
    if (scanf("%d %d", &linhas, &colunas) != 2)
        return 1;

    char **tabuleiro = criaTabuleiro(linhas, colunas);

    recebeTabuleiro(tabuleiro, linhas, colunas);
    lerComando(tabuleiro, linhas, colunas);

    // No final liberta a memoria
    for (int i = 0; i < linhas; i++)
        free(tabuleiro[i]);
    free(tabuleiro);

    return 0;
}

