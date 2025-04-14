#include <stdio.h>
#include <stdlib.h>
#include "tabuleiro.h"
#include "interface.h"
#include "stack.h"
#include "ficheiros.h"

int main(void) {
    int linhas, colunas;
    
    // Solicita as dimensões do tabuleiro ao utilizador.
    printf("Insira as dimensões do tabuleiro (ex: 5 5): ");
    if (scanf("%d %d", &linhas, &colunas) != 2) {
        printf("Dimensões inválidas.\n");
        return 1;
    }

    // Tenta ler do ficheiro "tabuleiros.txt" o tabuleiro com as dimensões especificadas.
    // O ficheiro deve estar no mesmo diretório onde o programa é executado.
    char **tabuleiro = lerTabuleiroPorDimensoes("tabuleiros.txt", &linhas, &colunas);
    if (!tabuleiro) {
        printf("Não foi encontrado um tabuleiro com dimensões %dx%d no ficheiro.\n", linhas, colunas);
        return 1;
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

    // Esvazia a stack (libera todos os nós) antes de terminar o programa.
    Movimento mov;
    while (pop(&stack, &mov)) {
        /* Apenas esvaziamos a stack */
    }

    return 0;
}
