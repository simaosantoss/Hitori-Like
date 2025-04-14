#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void initStack(StackMovimentos *stack) {
    stack->topo = NULL; 
}

/* Empilha um movimento */
void push(StackMovimentos *stack, Movimento mov) {
    Nodo *novo = malloc(sizeof(Nodo));
    if (!novo) return; // Falha de alocação, caso raro
    novo->mov = mov;
    novo->prox = stack->topo;
    stack->topo = novo;
}

/* Desempilha */
int pop(StackMovimentos *stack, Movimento *mov) {
    if (!stack->topo) {
        return 0; // stack vazia
    }
    Nodo *temp = stack->topo;
    *mov = temp->mov;  
    stack->topo = temp->prox;
    free(temp);
    return 1;
}
