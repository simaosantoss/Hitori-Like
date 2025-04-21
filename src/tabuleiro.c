#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
    // NÃO imprimir a primeira linha (linha 0)
    for (int i = 1; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

static int existeDuplicadoLinhaColuna(char **tab, int lin, int col, Coordenadas p, char letraMaius) {
    for (int j = 0; j < col; j++) {
        if (j != p.x && tab[p.y][j] == letraMaius) {
            return 1; 
        }
    }
    for (int i = 0; i < lin; i++) {
        if (i != p.y && tab[i][p.x] == letraMaius) {
            return 1;
        }
    }
    return 0;
}

static int vizinhosSaoBrancos(char **tab, int linhas, int colunas, Coordenadas p) {
    if (p.y - 1 >= 0 && !isupper(tab[p.y-1][p.x])) return 0;
    if (p.y + 1 < linhas && !isupper(tab[p.y+1][p.x])) return 0;
    if (p.x - 1 >= 0 && !isupper(tab[p.y][p.x-1])) return 0;
    if (p.x + 1 < colunas && !isupper(tab[p.y][p.x+1])) return 0;
    return 1;
}

int pintaBranco(char **tabuleiro, int linhas, int colunas, Coordenadas ponto) {
    // AJUSTAR COORDENADA: ignorar a primeira linha do tabuleiro
    ponto.y += 1;

    if (ponto.x < 0 || ponto.x >= colunas || ponto.y < 0 || ponto.y >= linhas) {
        printf("Coordenada fora do tabuleiro.\n");
        return 0;
    }
    char atual = tabuleiro[ponto.y][ponto.x];
    if (isupper(atual)) {
        printf("Erro: já está pintado de branco.\n");
        return 0;
    }
    if (atual == '#') {
        printf("Erro: não se pode pintar uma casa riscada.\n");
        return 0;
    }

    char maius = toupper(atual);
    if (existeDuplicadoLinhaColuna(tabuleiro, linhas, colunas, ponto, maius)) {
        printf("Erro: já existe essa letra em maiúscula na linha ou coluna.\n");
        return 0;
    }

    tabuleiro[ponto.y][ponto.x] = maius;
    return 1;
}

int riscaQuadrado(char **tabuleiro, int linhas, int colunas, Coordenadas ponto) {
    ponto.y += 1;

    if (ponto.x < 0 || ponto.x >= colunas || ponto.y < 0 || ponto.y >= linhas) {
        printf("Coordenada fora do tabuleiro.\n");
        return 0;
    }
    char atual = tabuleiro[ponto.y][ponto.x];
    if (atual == '#') {
        printf("Erro: já está riscado.\n");
        return 0;
    }
    if (isupper(atual)) {
        printf("Erro: não se pode riscar uma casa pintada de branco (faça fundo primeiro!).\n");
        return 0;
    }
    if (!vizinhosSaoBrancos(tabuleiro, linhas, colunas, ponto)) {
        printf("Erro: não pode riscar, pois alguma casa vizinha não está pintada de branco.\n");
        return 0;
    }

    tabuleiro[ponto.y][ponto.x] = '#';
    return 1;
}

int converteParaMinuscula(char **tabuleiro, int linhas, int colunas, Coordenadas ponto) {
    ponto.y += 1;

    if (ponto.x < 0 || ponto.x >= colunas || ponto.y < 0 || ponto.y >= linhas) {
        printf("Coordenada fora do tabuleiro.\n");
        return 0;
    }
    char atual = tabuleiro[ponto.y][ponto.x];
    if (!isupper(atual)) {
        printf("Erro: a casa não está em maiúscula, não faz sentido converter.\n");
        return 0;
    }
    tabuleiro[ponto.y][ponto.x] = tolower(atual);
    return 1;
}
