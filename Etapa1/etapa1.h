#ifndef ETAPA1_H
#define ETAPA1_H

typedef struct coordenadas {
    int x;
    int y;
} Coordenadas;

// Função para imprimir o tabuleiro
void imprimeTabuleiro(char a[5][5], int linhas, int colunas);

// Função para pintar de branco (tornar a letra maiúscula)
void pintaBranco(int linhas, int colunas, Coordenadas ponto);

// Função para riscar a casa (substituir por '#')
void riscaQuadrado(int linhas, int colunas, Coordenadas ponto);

#endif