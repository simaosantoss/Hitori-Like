#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ficheiros.h"

/**
 * Lê de um ficheiro (nomeFicheiro) vários tabuleiros,
 * cada um começando com "Linhas Colunas", seguido das linhas do tabuleiro.
 * Ao encontrar um tabuleiro com as dimensões *linhas e *colunas, lê e retorna.
 * Caso não encontre, retorna NULL.
 *
 * O ficheiro de entrada deve ter os blocos no seguinte formato:
 *
 *   L C
 *   linha1
 *   linha2
 *   ...
 *   linhaL
 *
 * Exemplo para um tabuleiro 5x5:
 *
 *   5 5
 *   ecadc
 *   dcdec
 *   bddce
 *   cdeeb
 *   accbb
 */
char **lerTabuleiroPorDimensoes(const char *nomeFicheiro, int *linhas, int *colunas) {
    FILE *f = fopen(nomeFicheiro, "r");
    if (!f) {
        printf("Erro ao abrir ficheiro '%s'.\n", nomeFicheiro);
        return NULL;
    }

    int L, C;
    // Percorre o ficheiro procurando um bloco com as dimensões desejadas
    while (fscanf(f, "%d %d", &L, &C) == 2) {
        if (L == *linhas && C == *colunas) {
            // Encontrámos as dimensões pedidas.
            // Aloca memória para o tabuleiro
            char **tab = malloc(L * sizeof(char *));
            if (!tab) {
                printf("Erro de alocação de memória.\n");
                fclose(f);
                return NULL;
            }
            for (int i = 0; i < L; i++) {
                tab[i] = malloc(C * sizeof(char));
                if (!tab[i]) {
                    printf("Erro de alocação de memória.\n");
                    for (int k = 0; k < i; k++) {
                        free(tab[k]);
                    }
                    free(tab);
                    fclose(f);
                    return NULL;
                }
            }
            // Ler as próximas L linhas, cada uma com C caracteres
            for (int i = 0; i < L; i++) {
                for (int j = 0; j < C; j++) {
                    if (fscanf(f, " %c", &tab[i][j]) != 1) {
                        printf("Ficheiro corrompido.\n");
                        for (int k = 0; k <= i; k++) {
                            free(tab[k]);
                        }
                        free(tab);
                        fclose(f);
                        return NULL;
                    }
                }
            }
            fclose(f);
            return tab; // Retorna o tabuleiro lido
        } else {
            // As dimensões não batem; ignora as próximas L linhas para avançar para o próximo bloco
            for (int skip = 0; skip < L; skip++) {
                char buffer[1024];
                if (fscanf(f, "%s", buffer) != 1) {
                    // Se não conseguir ler, sai do loop de skip
                    break;
                }
            }
        }
    }
    fclose(f);
    // Não encontrou um tabuleiro com as dimensões desejadas
    return NULL;
}

/**
 * Grava num ficheiro (nomeFicheiro) um tabuleiro, no formato:
 *
 *   L C
 *   linha1
 *   linha2
 *   ...
 *   linhaL
 *
 * Retorna 1 se sucesso ou 0 se falhar.
 */
int gravarEmFicheiro(const char *nomeFicheiro, char **tabuleiro, int linhas, int colunas) {
    FILE *f = fopen(nomeFicheiro, "w");
    if (!f) {
        printf("Erro ao abrir ficheiro %s para escrita.\n", nomeFicheiro);
        return 0;
    }
    // Escreve o número de linhas e colunas
    fprintf(f, "%d %d\n", linhas, colunas);
    // Escreve cada linha do tabuleiro
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fprintf(f, "%c", tabuleiro[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 1;
}
