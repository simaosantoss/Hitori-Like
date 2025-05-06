#include <stdlib.h>
#include "stack.h"

void initStack(StackMovimentos *s) { s->topo = NULL; }

/* operações */
void push(StackMovimentos *s, Movimento m)
{
    Nodo *n = malloc(sizeof *n);
    if (!n) return;
    n->mov  = m;                          /* copia o movimento */
    n->prox = s->topo;                    /* liga ao antigo topo */
    s->topo = n;                          /* este passa a ser o topo */
}

int pop(StackMovimentos *s, Movimento *m)
{
    if (!s->topo) return 0;               
    Nodo *n = s->topo;                    /* nó para remover */
    *m      = n->mov;                     /* copia dados */
    s->topo = n->prox;                    /* sobe o nó de baixo */
    free(n);                              /* devolve a memória */
    return 1;
}

/* destruição */
void destruirStack(StackMovimentos *s)
{
    Movimento tmp;
    while (pop(s, &tmp));    /* esvazia a lista e liberta os nós */
}
