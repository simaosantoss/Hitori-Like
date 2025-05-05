#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "interface.h"
#include "stack.h"
#include "ficheiros.h"

int main(void) {
    int linhas, colunas;
    char nomeFicheiro[64];  // Variável para armazenar o nome do ficheiro
    char comando[2];  // Variável para armazenar o comando 'l'

    char **tabuleiro = NULL;  // Declarar o tabuleiro fora do loop para ser acessível depois

    // Solicita obrigatoriamente o comando 'l <ficheiro>'
    while (1) {
        printf("Digite o comando 'l <ficheiro>' para ler o tabuleiro:\n");
        if (scanf(" %s %s", comando, nomeFicheiro) != 2) {
            printf("Comando inválido! Use 'l <ficheiro>' para ler o tabuleiro.\n");
            continue; // Solicita novamente o comando
        }

        if (strcmp(comando, "l") == 0) {
            // Tenta ler o tabuleiro do ficheiro fornecido.
            tabuleiro = lerTabuleiroFicheiro(nomeFicheiro, &linhas, &colunas);
            if (!tabuleiro) {
                printf("Erro ao ler o tabuleiro do ficheiro '%s'.\n", nomeFicheiro);
                continue;  // Solicita novamente o comando
            }
            break;  // Tabuleiro lido com sucesso, continua para os outros comandos
        } else {
            printf("Comando inválido! Use 'l <ficheiro>' para ler o tabuleiro.\n");
        }
    }

    // Cria e inicializa a stack que armazenará os movimentos (para implementar o comando de "undo").
    StackMovimentos stack;
    initStack(&stack);

    // Inicia o modo REPL (Read-Evaluate-Print Loop) onde o utilizador poderá interagir com o jogo.
    lerComando(&tabuleiro, &linhas, &colunas, &stack);

    // Libera a memória alocada para o tabuleiro.
    if (tabuleiro) {
        for (int i = 0; i < linhas; i++) {
            free(tabuleiro[i]);
        }
        free(tabuleiro);
    }

    // Esvazia a stack (liberta todos os nós) antes de terminar o programa.
    Movimento mov;
    while (pop(&stack, &mov)) {
        // Apenas esvaziamos a stack
    }

    return 0;
}
