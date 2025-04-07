#include <stdio.h>
#include <stdlib.h>
#include "etapa1.h"

// Estes comentarios depois sao apagados manos, isto é só para explicar o que fiz e para não estarem aqui às cegas
// esta versão segue exatamente o que é pedido no enunciado, mas se não perceberem o porque de algo vejam o enunciado mais concretamente a parte "Comandos a implementar"
// foi SEM chat, usei para perceber como ia implementar uma funcionalidade mas de resto foi sem chat
// é muito facil de perceber (se não fosse eu não sabia fazer) mas se tiverem duvidas é só perguntar
// caso haja algo que não gostem mudem ou se acharem que estava melhor antes é só ir ao histórico de commits e revertem

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

void lerComando(char **tabuleiro, int linhas, int colunas){
    char comando[10]; // Comando máximo esperado, por exemplo, "b a1". Meti 10 para não dar erro
    char letra;
    int numero;

    // Loop REPL
    while (1) {
        imprimeTabuleiro(tabuleiro, linhas, colunas);
        printf("Digite um comando (b <coordenada>, r <coordenada>, s para sair): \n");

        if (scanf(" %s", comando) != 1)
            continue;

        if (comando[0] == 's') {
            printf("A sair do programa...\n");
            break;
        }

        if (comando[0] == 'b' || comando[0] == 'r') {
            if (scanf(" %c%d", &letra, &numero) != 2)
                continue;

            Coordenadas ponto;
            ponto.x = letra - 'a';
            ponto.y = numero - 1;

            if (comando[0] == 'b')
                pintaBranco(tabuleiro, linhas, colunas, ponto);
            else if (comando[0] == 'r')
                riscaQuadrado(tabuleiro, linhas, colunas, ponto);
        } else {
            printf("Comando inválido!\n");
        }
    }
}

