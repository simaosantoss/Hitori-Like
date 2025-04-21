#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h" 
/**
 * Lê um tabuleiro de um ficheiro de texto.
 * Cada linha do ficheiro representa uma linha do tabuleiro.
 * Todas as linhas devem ter o mesmo número de colunas.
 * Guarda o número de linhas e colunas nas variáveis recebidas.
 */
char **lerTabuleiroFicheiro(const char *nomeFicheiro, int *linhas, int *colunas) {
    FILE *ficheiro = fopen(nomeFicheiro, "r");
    if (!ficheiro) {
        printf("Erro ao abrir o ficheiro '%s'.\n", nomeFicheiro);
        return NULL;
    }

    char linha[512];
    int totalLinhas = 0;
    int totalColunas = 0;

    // Contar linhas e colunas
    while (fgets(linha, sizeof(linha), ficheiro)) {
        int tamanho = strlen(linha);
        if (linha[tamanho - 1] == '\n') linha[tamanho - 1] = '\0';

        if (totalLinhas == 0) {
            totalColunas = strlen(linha); // Armazena a largura (número de colunas) da primeira linha
        }
        totalLinhas++; // Conta o número de linhas
    }

    // Reposiciona o ponteiro de leitura para o início do arquivo
    rewind(ficheiro);

    // Criar o tabuleiro com as dimensões determinadas, reutilizando criaTabuleiro
    char **tabuleiro = criaTabuleiro(totalLinhas, totalColunas);

    // Preencher o tabuleiro com os dados do arquivo
    int linhaAtual = 0;
    while (fgets(linha, sizeof(linha), ficheiro) && linhaAtual < totalLinhas) {
        for (int j = 0; j < totalColunas; j++) {
            tabuleiro[linhaAtual][j] = linha[j]; // Preenche a linha do tabuleiro
        }
        linhaAtual++;
    }

    fclose(ficheiro);

    *linhas = totalLinhas;   // Armazena o número de linhas
    *colunas = totalColunas; // Armazena o número de colunas
    return tabuleiro;         // Retorna o tabuleiro preenchido
}

/**
 * Guarda um tabuleiro num ficheiro.
 * Formato:
 *   L C
 *   linha1
 *   linha2
 *   ...
 */
int gravarTabuleiroFicheiro(const char *nomeFicheiro, char **tabuleiro, int linhas, int colunas) {
    FILE *ficheiro = fopen(nomeFicheiro, "w");
    if (!ficheiro) {
        printf("Erro ao abrir o ficheiro '%s' para escrita.\n", nomeFicheiro);
        return 0;
    }

    fprintf(ficheiro, "%d %d\n", linhas, colunas);

    // Escrever as linhas do tabuleiro
    for (int i = 1; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fputc(tabuleiro[i][j], ficheiro);
        }
        fputc('\n', ficheiro);
    }

    fclose(ficheiro);
    return 1;
}

/**
 * Liberta a memória alocada para o tabuleiro.
 * Recebe o ponteiro para o tabuleiro, o número de linhas.
 */
void libertaMemoria(char **tabuleiro, int linhas) {
    // Libera a memória alocada para cada linha do tabuleiro
    for (int i = 0; i < linhas; i++) {
        free(tabuleiro[i]);
    }

    // Libera o ponteiro principal do tabuleiro
    free(tabuleiro);
}
