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

/* --- API esperada pelos testes --- */
void inicializarStack(StackMovimentos *s);   /* novo alias */
void destruirStack(StackMovimentos *s);      /* libertar toda a memória */

void initStack(StackMovimentos *s);          /* nome usado no main */
void push(StackMovimentos *s, Movimento m);
int  pop (StackMovimentos *s, Movimento *m);

#endif /* STACK_H */
