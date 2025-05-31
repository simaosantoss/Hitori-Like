#ifndef STACK_H
#define STACK_H

typedef struct {
    int x, y;
    char valorAntigo, valorNovo;
} Movimento;

typedef struct Nodo {
    Movimento mov;
    struct Nodo *prox;
} Nodo;

typedef struct {
    Nodo *topo;
} StackMovimentos;

void destruirStack(StackMovimentos *s);      /* libertar toda a memória */
void initStack(StackMovimentos *s);          /* nome usado no main */

void push(StackMovimentos *s, Movimento m);
int  pop (StackMovimentos *s, Movimento *m);

#endif 