#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../include/tabuleiro.h"

/* criação / IO simples */
char **criaTabuleiro(int linhas, int colunas) {
    char **t = malloc(linhas * sizeof *t);
    for (int i = 0; i < linhas; ++i)
        t[i] = malloc(colunas * sizeof **t);
    return t;
}

void imprimeTabuleiro(char **t, int l, int c) {
    for (int y = 0; y < l; ++y) {
        for (int x = 0; x < c; ++x) {
            putchar(t[y][x]);
            putchar(' ');
        }
        putchar('\n');
    }
}

/* primitivas de edição */
int pintaBranco(char **t, int l, int c, Coordenadas p) {
    if (p.x < 0 || p.x >= c || p.y < 0 || p.y >= l) {
        puts("As coordenadas estão fora do tabuleiro.");
        return 0;
    }
    if (isupper(t[p.y][p.x])) {
        puts("A casa já está pintada de branco e não pode ser pintada novamente.");
        return 0;
    }
    if (t[p.y][p.x] == '#') {
        puts("Uma casa riscada não pode ser pintada de branco.");
        return 0;
    }
    t[p.y][p.x] = toupper(t[p.y][p.x]);
    return 1;
}

int riscaQuadrado(char **t, int l, int c, Coordenadas p) {
    if (p.x < 0 || p.x >= c || p.y < 0 || p.y >= l) {
        puts("As coordenadas estão fora do tabuleiro.");
        return 0;
    }
    if (t[p.y][p.x] == '#') {
        puts("A casa já está riscada e não pode ser riscada novamente.");
        return 0;
    }
    if (isupper(t[p.y][p.x])) {
        puts("Uma casa branca não pode ser riscada.");
        return 0;
    }
    t[p.y][p.x] = '#';
    return 1;
}

int converteParaMinuscula(char **t, Coordenadas p) {
    if (!isupper(t[p.y][p.x])) {
        puts("A letra da casa selecionada não é maiúscula.");
        return 0;
    }
    t[p.y][p.x] = tolower(t[p.y][p.x]);
    return 1;
}

/* implementação do comando a */
static int risca_replicas(char **tab, int l, int c) {
    int mudou = 0;
    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (isupper(tab[y][x])) {
                char target = tolower(tab[y][x]);
                /* linha */
                for (int xx = 0; xx < c; ++xx)
                    if (islower(tab[y][xx]) && tab[y][xx] == target)
                        tab[y][xx] = '#', mudou = 1;
                /* coluna */
                for (int yy = 0; yy < l; ++yy)
                    if (islower(tab[yy][x]) && tab[yy][x] == target)
                        tab[yy][x] = '#', mudou = 1;
            }
    return mudou;
}

static int pinta_vizinhos_hash(char **tab, int l, int c) {
    static const int dx[4] = {1, -1, 0, 0}, dy[4] = {0, 0, 1, -1};
    int mudou = 0;
    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (tab[y][x] == '#')
                for (int k = 0; k < 4; ++k) {
                    int nx = x + dx[k], ny = y + dy[k];
                    if (nx < 0 || nx >= c || ny < 0 || ny >= l)
                        continue;
                    if (islower(tab[ny][nx])) {
                        tab[ny][nx] = toupper(tab[ny][nx]);
                        mudou = 1;
                    }
                }
    return mudou;
}

int aplica_comando_a(char **tab, int l, int c) {
    int mudou1 = risca_replicas(tab, l, c);
    int mudou2 = pinta_vizinhos_hash(tab, l, c);
    return mudou1 || mudou2;
}

int aplica_comando_A(char **tab, int l, int c) {
    int total = 0;
    while (aplica_comando_a(tab, l, c))
        total++;
    return total;
}

/* helpers para o solver (R) */
char **duplicaTabuleiro(char **src, int l, int c) {
    char **new = criaTabuleiro(l, c);
    for (int y = 0; y < l; ++y)
        memcpy(new[y], src[y], c);
    return new;
}

void copiaTabuleiro(char **dst, char **src, int l, int c) {
    for (int y = 0; y < l; ++y)
        memcpy(dst[y], src[y], c);
}

static int tem_minusculas(char **t, int l, int c) {
    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (islower(t[y][x]))
                return 1;
    return 0;
}

static Coordenadas escolhe_celula(char **t, int l, int c) {
    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (islower(t[y][x])) {
                char ch = t[y][x];
                for (int xx = 0; xx < c; ++xx)
                    if (xx != x && (t[y][xx] == ch || t[y][xx] == toupper(ch)))
                        return (Coordenadas){x, y};
                for (int yy = 0; yy < l; ++yy)
                    if (yy != y && (t[yy][x] == ch || t[yy][x] == toupper(ch)))
                        return (Coordenadas){x, y};
            }
    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (islower(t[y][x]))
                return (Coordenadas){x, y};
    return (Coordenadas){-1, -1};
}

/* backtracking (comando R) */
int resolverJogo(char **tab, int l, int c) {
    aplica_comando_A(tab, l, c);                 /* 1º propagação */
    if (!regrasBasicasOk(tab, l, c))
        return 0;                                /* descarta ramos impossíveis */

    if (!tem_minusculas(tab, l, c))              /* se já não há incógnitas... */
        return verificaEstado(tab, l, c);        /* ...verifica ligação final */

    Coordenadas cel = escolhe_celula(tab, l, c);

    /* 1) supor branco */
    {
        char **tmp = duplicaTabuleiro(tab, l, c);
        pintaBranco(tmp, l, c, cel);
        if (resolverJogo(tmp, l, c)) {
            copiaTabuleiro(tab, tmp, l, c);
            for (int i = 0; i < l; ++i)
                free(tmp[i]);
            free(tmp);
            return 1;
        }
        for (int i = 0; i < l; ++i)
            free(tmp[i]);
        free(tmp);
    }

    /* 2) supor # */
    {
        char **tmp = duplicaTabuleiro(tab, l, c);
        riscaQuadrado(tmp, l, c, cel);
        if (resolverJogo(tmp, l, c)) {
            copiaTabuleiro(tab, tmp, l, c);
            for (int i = 0; i < l; ++i)
                free(tmp[i]);
            free(tmp);
            return 1;
        }
        for (int i = 0; i < l; ++i)
            free(tmp[i]);
        free(tmp);
    }

    return 0;                                    /* nenhum ramo resultou */
}
