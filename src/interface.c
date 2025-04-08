#include <stdio.h>
#include <stdlib.h>
#include "tabuleiro.h"
#include "interface.h"

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

