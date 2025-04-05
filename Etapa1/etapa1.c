#include <stdio.h>

typedef struct coordenadas{
    int x;
    int y;
} Coordenadas;

char tabuleiro[5][5] = {
    {'e', 'c', 'a', 'd', 'c'},
    {'d', 'c', 'd', 'e', 'c'},
    {'b', 'd', 'd', 'c', 'e'},
    {'c', 'd', 'e', 'e', 'b'},
    {'a', 'c', 'c', 'b', 'b'}
};

void imprimeTabuleiro(char a[5][5], int linhas, int colunas){
    for (int i = 0; i < linhas; i++){
        for (int j = 0; j < colunas; j++){
        printf("%c  ", a[i][j]);
    }
        printf("\n");
    }
}

void pintaBranco(int linhas, int colunas){
    Coordenadas ponto;
    imprimeTabuleiro(tabuleiro, 5, 5);
    printf("Que casa pretende tornar maiúscula? \nInsira as coordenadas:");
    if (scanf("%d %d", &ponto.x, &ponto.y) != 2) {
    printf("Erro! Input incorreto.");
    return;
}
if (ponto.x < 0 || ponto.x >= colunas || ponto.y < 0 || ponto.y >= linhas) {
    printf("Coordenadas fora do limite!\n");
    return;
    
    for (int i = 1; i < linhas; i++){
        for (int j = 1; j < colunas; j++){
            if (i == ponto.y && j == ponto.x)
            tabuleiro[i-1][j-1]= tabuleiro[i-1][j-1] - 32;
    }
    }
}
}

void riscaQuadrado(int linhas, int colunas){
    Coordenadas ponto;
    imprimeTabuleiro(tabuleiro, 5, 5);
    printf("Que casa pretende riscar? \nInsira as coordenadas:");
    if (scanf("%d %d", &ponto.x, &ponto.y) != 2) {
    printf("Erro! Input incorreto.");
    return;
}
    for (int i = 1; i < linhas; i++){
        for (int j = 1; j < colunas; j++){
            if (i == ponto.y && j == ponto.x)
            tabuleiro[i-1][j-1]= '#';
    }
    }
    imprimeTabuleiro(tabuleiro, 5, 5);
}

int main () {
    pintaBranco(5, 5);
    riscaQuadrado(5, 5);
    return 0;
}