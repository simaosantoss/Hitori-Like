#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "interface.h"
#include "stack.h"
#include "ficheiros.h"

int main(void) {
    int   linhas, colunas;
    char  nomeFicheiro[64], comando[2];
    char **tabuleiro = NULL;

    /* Obriga a carregar um puzzle logo à partida */
    while (1) {
        printf("Digite o comando 'l <ficheiro>' para ler o tabuleiro:\n");
        if (scanf(" %s %s", comando, nomeFicheiro) != 2) {
            puts("Comando inválido! Use 'l <ficheiro>'.");
            continue;
        }
        if (strcmp(comando, "l") == 0) {
            tabuleiro = lerTabuleiroFicheiro(nomeFicheiro, &linhas, &colunas);
            if (!tabuleiro) {
                printf("Erro ao ler '%s'.\n", nomeFicheiro);
                continue;
            }
            break;
        }
        puts("Comando inválido! Use 'l <ficheiro>'.");
    }

    /* Guarda a versão imaculada para o comando R */
    guardaOriginal(tabuleiro, linhas, colunas);

    /* Stack para undo */
    StackMovimentos stack;
    initStack(&stack);

    /* Ciclo REPL */
    lerComando(&tabuleiro, &linhas, &colunas, &stack);

    /* Libertação de memória */
    if (tabuleiro) libertaMemoria(tabuleiro, linhas);
    destruirStack(&stack);
    return 0;
}
