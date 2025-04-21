#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h" 

/**
 * Liberta a memória alocada para o tabuleiro.
 * Recebe o ponteiro para o tabuleiro, o número de linhas.
 */
void libertaMemoria(char **tabuleiro, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
}

/**
 * Lê um tabuleiro de um ficheiro de texto.
 * A primeira linha do ficheiro com as dimensões (linhas colunas) é ignorada.
 * As seguintes linhas representam o tabuleiro real.
 */
char **lerTabuleiroFicheiro(const char *nomeFicheiro, int *linhas, int *colunas) {
    FILE *ficheiro = fopen(nomeFicheiro, "r");
    if (!ficheiro) {
        printf("Erro ao abrir o ficheiro '%s'.\n", nomeFicheiro);
        return NULL;
    }

    // Lê e ignora a primeira linha (dimensões do tabuleiro)
    char buffer[512];
    if (!fgets(buffer, sizeof(buffer), ficheiro)) {
        printf("Erro: ficheiro vazio ou inválido.\n");
        fclose(ficheiro);
        return NULL;
    }

    // Contar as linhas e colunas do tabuleiro real (ignorando a primeira linha)
    int totalLinhas = 0;
    int totalColunas = 0;
    long posInicioTabuleiro = ftell(ficheiro); // guarda posição onde começa o tabuleiro

    while (fgets(buffer, sizeof(buffer), ficheiro)) {
        int tamanho = strlen(buffer);
        if (buffer[tamanho - 1] == '\n') buffer[tamanho - 1] = '\0';
        if (totalColunas == 0) {
            totalColunas = strlen(buffer);
        }
        totalLinhas++;
    }

    // Reposiciona para início do tabuleiro
    fseek(ficheiro, posInicioTabuleiro, SEEK_SET);

    // Cria o tabuleiro
    char **tabuleiro = criaTabuleiro(totalLinhas, totalColunas);
    if (!tabuleiro) {
        fclose(ficheiro);
        return NULL;
    }

    int linhaAtual = 0;
    while (fgets(buffer, sizeof(buffer), ficheiro) && linhaAtual < totalLinhas) {
        for (int j = 0; j < totalColunas; j++) {
            tabuleiro[linhaAtual][j] = buffer[j];
        }
        linhaAtual++;
    }

    fclose(ficheiro);

    *linhas = totalLinhas;
    *colunas = totalColunas;
    return tabuleiro;
}

/**
 * Guarda um tabuleiro num ficheiro.
 * Primeira linha com o número de linhas e colunas.
 * Segue-se o conteúdo do tabuleiro.
 */
int gravarTabuleiroFicheiro(const char *nomeFicheiro, char **tabuleiro, int linhas, int colunas) {
    FILE *ficheiro = fopen(nomeFicheiro, "w");
    if (!ficheiro) {
        printf("Erro ao abrir o ficheiro '%s' para escrita.\n", nomeFicheiro);
        return 0;
    }

    fprintf(ficheiro, "%d %d\n", linhas, colunas);

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fputc(tabuleiro[i][j], ficheiro);
        }
        fputc('\n', ficheiro);
    }

    fclose(ficheiro);
    return 1;
}
