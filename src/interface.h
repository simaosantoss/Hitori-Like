#ifndef INTERFACE_H
#define INTERFACE_H

#include "ficheiros.h"   /* para libertaMemoria */
#include "stack.h"       /* para StackMovimentos */

/* protótipo que já existe em interface.c */
void lerComando(char ***ptab, int *lin, int *col, StackMovimentos *stk);

#endif /* INTERFACE_H */
