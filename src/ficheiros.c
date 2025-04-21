#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * Cria um tabuleiro com o número especificado de linhas e colunas.
 * Aloca memória para o tabuleiro e inicializa cada posição.
 */
char **criarTabuleiro(int linhas, int colunas) {
    char **tabuleiro = (char **)malloc(linhas * sizeof(char *));
    if (!tabuleiro) {
        printf("Erro ao alocar memória para o tabuleiro.\n");
        return NULL;
    }

    for (int i = 0; i < linhas; i++) {
        tabuleiro[i] = (char *)malloc(colunas * sizeof(char));
        if (!tabuleiro[i]) {
            printf("Erro ao alocar memória para a linha %d do tabuleiro.\n", i);
            // Libera memória alocada até o momento
            for (int j = 0; j < i; j++) {
                free(tabuleiro[j]);
            }
            free(tabuleiro);
            return NULL;
        }
    }

    // Inicializa o tabuleiro com espaços vazios ou algum valor padrão
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = ' ';  // Inicializa com um espaço vazio
        }
    }

    return tabuleiro;
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
    char **tabuleiro = criarTabuleiro(totalLinhas, totalColunas);
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
