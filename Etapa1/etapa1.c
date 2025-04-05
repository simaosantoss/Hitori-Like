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
    for (int i = 0; i < linhas; i++){
        for (int j = 0; j < colunas; j++){
            if (i == ponto.y && j == ponto.x)
            tabuleiro[i][j]= tabuleiro[i][j] - 32;
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
    for (int i = 0; i < linhas; i++){
        for (int j = 0; j < colunas; j++){
            if (i == ponto.y && j == ponto.x)
            tabuleiro[i][j]= '#';
    }
    }
    imprimeTabuleiro(tabuleiro, 5, 5);
}

int main () {
    pintaBranco(5, 5);
    riscaQuadrado(5, 5);
    return 0;
}