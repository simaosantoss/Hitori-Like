#ifndef INTERFACE_H
#define INTERFACE_H

#include "stack.h"

/* Lê comandos REPL e manipula o tabuleiro. Usa a stack para undo. */
void lerComando(char ***tabuleiro, int *linhas, int *colunas, StackMovimentos *stack);

#endif
