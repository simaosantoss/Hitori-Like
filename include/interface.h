#ifndef INTERFACE_H
#define INTERFACE_H
#include "ficheiros.h"  
#include "stack.h"       

void lerComando(char ***ptab, int *lin, int *col, StackMovimentos *stk);

void guardaOriginal(char **tab, int l, int c);

#endif
