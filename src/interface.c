#include <stdio.h>
#include <stdlib.h>
#include <string.h> // para strcmp
#include "tabuleiro.h"
#include "interface.h"
#include "ficheiros.h"
#include "stack.h"

void lerComando(char ***ptabuleiro, int *linhas, int *colunas, StackMovimentos *stack) {
    // ptabuleiro é um ponteiro para o 'char**' real, pois podemos ter de realocar se ler de ficheiro
    char comando[64];
    char letra;
    int numero;

    while (1) {
        imprimeTabuleiro(*ptabuleiro, *linhas, *colunas);
        printf("Digite um comando (b <coord>, r <coord>, f <coord>, l <fic>, g <fic>, d, s, ...):\n");
        if (scanf(" %s", comando) != 1) {
            continue;
        }
        if (strcmp(comando, "s") == 0) {
            printf("A sair do programa...\n");
            break;
        }
        else if (strcmp(comando, "b") == 0) {
            // pintar de branco
            if (scanf(" %c%d", &letra, &numero) != 2) {
                printf("Coordenada inválida!\n");
                continue;
            }
            Coordenadas c = { letra - 'a', numero - 1 };
            char valorAntigo = (*ptabuleiro)[c.y][c.x];
            if (pintaBranco(*ptabuleiro, *linhas, *colunas, c)) {
                // Se deu certo, empilha
                Movimento mov;
                mov.x = c.x; mov.y = c.y;
                mov.valorAntigo = valorAntigo;
                mov.valorNovo = (*ptabuleiro)[c.y][c.x];
                push(stack, mov);
            }
        }
        else if (strcmp(comando, "r") == 0) {
            // riscar
            if (scanf(" %c%d", &letra, &numero) != 2) {
                printf("Coordenada inválida!\n");
                continue;
            }
            Coordenadas c = { letra - 'a', numero - 1 };
            char valorAntigo = (*ptabuleiro)[c.y][c.x];
            if (riscaQuadrado(*ptabuleiro, *linhas, *colunas, c)) {
                // se deu certo, empilha
                Movimento mov;
                mov.x = c.x; mov.y = c.y;
                mov.valorAntigo = valorAntigo;
                mov.valorNovo = (*ptabuleiro)[c.y][c.x];
                push(stack, mov);
            }
        }
        else if (strcmp(comando, "f") == 0) {
            // converter para minúscula
            if (scanf(" %c%d", &letra, &numero) != 2) {
                printf("Coordenada inválida!\n");
                continue;
            }
            Coordenadas c = { letra - 'a', numero - 1 };
            char valorAntigo = (*ptabuleiro)[c.y][c.x];
            if (converteParaMinuscula(*ptabuleiro, *linhas, *colunas, c)) {
                // se deu certo, empilha
                Movimento mov;
                mov.x = c.x; mov.y = c.y;
                mov.valorAntigo = valorAntigo;
                mov.valorNovo = (*ptabuleiro)[c.y][c.x];
                push(stack, mov);
            }
        }
        else if (strcmp(comando, "d") == 0) {
            // desfazer
            Movimento mov;
            if (pop(stack, &mov)) {
                // Reverte no tabuleiro
                (*ptabuleiro)[mov.y][mov.x] = mov.valorAntigo;
            } else {
                printf("Stack de movimentos vazia, nada a desfazer.\n");
            }
        }
        else if (strcmp(comando, "l") == 0) {
            // ler tabuleiro de ficheiro
            char nomeFich[64];
            if (scanf(" %s", nomeFich) != 1) {
                printf("Ficheiro inválido!\n");
                continue;
            }
            // Libertamos tabuleiro antigo
            for (int i = 0; i < *linhas; i++) {
                free((*ptabuleiro)[i]);
            }
            free(*ptabuleiro);

            // Lemos do ficheiro
            char **novo = lerTabuleiroPorDimensoes(nomeFich, linhas, colunas);
            if (!novo) {
                // Falhou, repor algo?
                *linhas = *colunas = 0;
                *ptabuleiro = NULL;
                printf("Erro ao ler ficheiro.\n");
            } else {
                *ptabuleiro = novo;
                // Zerar stack pois é um novo jogo
                while (pop(stack, & (Movimento){0})) {}
            }
        }
        else if (strcmp(comando, "g") == 0) {
            // gravar tabuleiro
            char nomeFich[64];
            if (scanf(" %s", nomeFich) != 1) {
                printf("Ficheiro inválido!\n");
                continue;
            }
            gravarEmFicheiro(nomeFich, *ptabuleiro, *linhas, *colunas);
        }
        else {
            printf("Comando inválido!\n");
        }
    }
}
