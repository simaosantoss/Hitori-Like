#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stddef.h>

/* estruturas básicas */
typedef struct { int x, y; } Coordenadas;

/* API do tabuleiro */
char **criaTabuleiro    (int linhas, int colunas);
void   imprimeTabuleiro (char **tab,int l,int c);

/* edição de células */
int    pintaBranco      (char **tab,int l,int c,Coordenadas p);   /* torna maiúscula */
int    riscaQuadrado    (char **tab,int l,int c,Coordenadas p);   /* coloca # */
int converteParaMinuscula(char **t,Coordenadas p);

/* verificação global (‘v’) */
int    verificaEstado   (char **tab,int l,int c);

/* comandos do projeto */
int    aplica_comando_a (char **tab,int l,int c);
int    aplica_comando_A (char **tab,int l,int c);
int    resolverJogo     (char **tab,int l,int c);

/* utilitários */
char **duplicaTabuleiro (char **src,int l,int c);
void   copiaTabuleiro   (char **dst,char **src,int l,int c);


int    verificaEstado   (char **tab,int l,int c);   /* completa Finalmente (!!) (inclui conectividade) */
int    regrasBasicasOk  (char **tab,int l,int c);   /* sem conectividade – para o R (estava a dar erros) */

#endif
