#ifndef TABULEIRO_H
#define TABULEIRO_H

typedef struct coordenadas {
    int x; 
    int y; 
} Coordenadas;

/**
 * Cria espaço para o tabuleiro.
 */
char **criaTabuleiro(int linhas, int colunas);

/**
 * - Não vamos usar muito isto pois o prof pediu para ler de ficheiro.
 */
void recebeTabuleiro(char **tabuleiro, int linhas, int colunas);

/**
 * Imprime o tabuleiro no stdout.
 */
void imprimeTabuleiro(char **tabuleiro, int linhas, int colunas);

/**
 * Pinta de branco a casa [ponto.y][ponto.x], retornando 1 se OK, 0 se erro.
 * - Verifica se já existe outra maiúscula igual na mesma linha ou coluna.
 * - Verifica se não está riscada, etc.
 */
int pintaBranco(char **tabuleiro, int linhas, int colunas, Coordenadas ponto);

/**
 * Risca a casa [ponto.y][ponto.x], retornando 1 se OK, 0 se erro.
 * - Se estiver maiúscula, devolve erro (tem que ser minúscula antes).
 * - Verifica adjacências, etc.
 */
int riscaQuadrado(char **tabuleiro, int linhas, int colunas, Coordenadas ponto);

/**
 * Converte a casa de maiúscula p/ minúscula (fundo),
 * retornando 1 se OK, 0 se erro.
 * - Se não estiver maiúscula, retorna 0.
 */
int converteParaMinuscula(char **tabuleiro, int linhas, int colunas, Coordenadas ponto);

#endif
