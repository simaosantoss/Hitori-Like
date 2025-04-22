#include <stdlib.h>
#include "stack.h"

/* -------- inicialização -------- */
void inicializarStack(StackMovimentos *s) { s->topo = NULL; }
void initStack        (StackMovimentos *s) { inicializarStack(s); }

/* -------- operações -------- */
void push(StackMovimentos *s, Movimento m)
{
    Nodo *n = malloc(sizeof *n);
    if (!n) return;
    n->mov  = m;
    n->prox = s->topo;
    s->topo = n;
}

int pop(StackMovimentos *s, Movimento *m)
{
    if (!s->topo) return 0;
    Nodo *n = s->topo;
    *m      = n->mov;
    s->topo = n->prox;
    free(n);
    return 1;
}

/* -------- destruição -------- */
void destruirStack(StackMovimentos *s)
{
    Movimento tmp;
    while (pop(s, &tmp));    /* esvazia a lista e liberta nós */
}
