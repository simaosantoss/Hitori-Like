#ifndef STACK_H
#define STACK_H

typedef struct movimento {
    int x, y;          // coordenadas do movimento
    char valorAntigo;  // o que estava antes
    char valorNovo;    // o que foi colocado
} Movimento;

typedef struct nodo {
    Movimento mov;
    struct nodo *prox;
} Nodo;

typedef struct {
    Nodo *topo;
} StackMovimentos;

/**
 * Inicializa a stack (ponteiro para NULL)
 */
void initStack(StackMovimentos *stack);

/**
 * Empilha um movimento
 */
void push(StackMovimentos *stack, Movimento mov);

/**
 * Desempilha (retorna 1 se OK, 0 se stack vazia)
 */
int pop(StackMovimentos *stack, Movimento *mov);

#endif
