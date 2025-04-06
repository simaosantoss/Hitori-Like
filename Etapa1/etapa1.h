#ifndef ETAPA1_H
#define ETAPA1_H

typedef struct coordenadas {
    int x;
    int y;
} Coordenadas;

char **criaTabuleiro(int linhas, int colunas);

void imprimeTabuleiro(char **tabuleiro, int linhas, int colunas);

void pintaBranco(char **tabuleiro, int linhas, int colunas, Coordenadas ponto);

void riscaQuadrado(char **tabuleiro, int linhas, int colunas, Coordenadas ponto);

void lerComando(char **tabuleiro, int linhas, int colunas);

#endif
