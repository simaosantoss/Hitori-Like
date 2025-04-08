#include <stdio.h>
#include <stdlib.h>
#include "tabuleiro.h"

char **criaTabuleiro(int linhas, int colunas){
    char **tabuleiro = malloc(linhas * sizeof(char *));
    for (int i = 0; i < linhas; i++)
        tabuleiro[i] = malloc(colunas * sizeof(char));
    return tabuleiro;
}

void recebeTabuleiro(char **tabuleiro, int linhas, int colunas){
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            if (scanf(" %c", &tabuleiro[i][j]) != 1)
                return;
}

void imprimeTabuleiro(char **tabuleiro, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++)
            printf("%c  ", tabuleiro[i][j]);
        printf("\n");
    }
}

void pintaBranco(char **tabuleiro, int linhas, int colunas, Coordenadas ponto) {
    if (ponto.x >= 0 && ponto.x < colunas && ponto.y >= 0 && ponto.y < linhas)
        tabuleiro[ponto.y][ponto.x] = tabuleiro[ponto.y][ponto.x] - 32;
    else
        printf("Coordenada inválida!\n");
}

void riscaQuadrado(char **tabuleiro, int linhas, int colunas, Coordenadas ponto) {
    if (ponto.x >= 0 && ponto.x < colunas && ponto.y >= 0 && ponto.y < linhas)
        tabuleiro[ponto.y][ponto.x] = '#';
    else
        printf("Coordenada inválida!\n");
}
