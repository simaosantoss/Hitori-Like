#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tabuleiro.h"

/* CriaTabuleiro e recebeTabuleiro inalterados, apenas comentários extras */
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
    // Apenas um print simples
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

/* Função auxiliar para verificar se já existe a mesma letra maiúscula
   na linha ou coluna do ponto. Se existir, retorna 1 (verdadeiro),
   senão retorna 0. */
static int existeDuplicadoLinhaColuna(char **tab, int lin, int col, Coordenadas p, char letraMaius) {
    // verifica na linha p.y
    for (int j = 0; j < col; j++) {
        if (j != p.x && tab[p.y][j] == letraMaius) {
            return 1; 
        }
    }
    // verifica na coluna p.x
    for (int i = 0; i < lin; i++) {
        if (i != p.y && tab[i][p.x] == letraMaius) {
            return 1;
        }
    }
    return 0;
}

/* Verificar se as casas vizinhas (cima, baixo, esquerda, direita) 
   de (p.y, p.x) estão pintadas de branco. Se alguma não estiver, retorna 0. */
static int vizinhosSaoBrancos(char **tab, int linhas, int colunas, Coordenadas p) {
    // cima
    if (p.y - 1 >= 0 && !isupper(tab[p.y-1][p.x])) return 0;
    // baixo
    if (p.y + 1 < linhas && !isupper(tab[p.y+1][p.x])) return 0;
    // esquerda
    if (p.x - 1 >= 0 && !isupper(tab[p.y][p.x-1])) return 0;
    // direita
    if (p.x + 1 < colunas && !isupper(tab[p.y][p.x+1])) return 0;
    return 1;
}

int pintaBranco(char **tabuleiro, int linhas, int colunas, Coordenadas ponto) {
    if (ponto.x < 0 || ponto.x >= colunas || ponto.y < 0 || ponto.y >= linhas) {
        printf("Coordenada fora do tabuleiro.\n");
        return 0;
    }
    char atual = tabuleiro[ponto.y][ponto.x];
    if (isupper(atual)) {
        // Já está pintado
        printf("Erro: já está pintado de branco.\n");
        return 0;
    }
    if (atual == '#') {
        // Já está riscado
        printf("Erro: não se pode pintar uma casa riscada.\n");
        return 0;
    }

    // Vamos transformar em maiúscula e verificar se há duplicados na mesma linha/coluna
    char maius = toupper(atual);
    if (existeDuplicadoLinhaColuna(tabuleiro, linhas, colunas, ponto, maius)) {
        printf("Erro: já existe essa letra em maiúscula na linha ou coluna.\n");
        return 0;
    }

    // Se passou nas verificações, pinta
    tabuleiro[ponto.y][ponto.x] = maius;
    return 1;
}

int riscaQuadrado(char **tabuleiro, int linhas, int colunas, Coordenadas ponto) {
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
        // Segundo a regra do professor, não podemos riscar direto
        printf("Erro: não se pode riscar uma casa pintada de branco (faça fundo primeiro!).\n");
        return 0;
    }
    // riscar exige que todas as vizinhas ortogonais estejam em maiúscula?
    // Se for regra, verificamos:
    if (!vizinhosSaoBrancos(tabuleiro, linhas, colunas, ponto)) {
        printf("Erro: não pode riscar, pois alguma casa vizinha não está pintada de branco.\n");
        return 0;
    }

    tabuleiro[ponto.y][ponto.x] = '#';
    return 1;
}

int converteParaMinuscula(char **tabuleiro, int linhas, int colunas, Coordenadas ponto) {
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
