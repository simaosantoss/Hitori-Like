#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../include/tabuleiro.h"

/*   Conectividade das casas brancas */
int casasBrancasConectadas(char **tab, int l, int c) {
    int total = 0, sx = -1, sy = -1;

    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (isupper(tab[y][x])) {
                total++;
                if (sx == -1) {
                    sx = x;
                    sy = y;
                }
            }

    if (total == 0)
        return 1; /* não há brancas -> ligadas */

    bool *vis = calloc(l * c, sizeof *vis);
    int  *qx  = malloc(l * c * sizeof *qx);
    int  *qy  = malloc(l * c * sizeof *qy);
    int head  = 0, tail = 0;

    qx[tail] = sx;
    qy[tail] = sy;
    tail++;
    vis[sy * c + sx] = true;

    int visit = 0;
    const int dx[4] = {1, -1, 0, 0},
              dy[4] = {0, 0, 1, -1};

    while (head < tail) {
        int x = qx[head], y = qy[head];
        head++;
        visit++;

        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k], ny = y + dy[k];
            if (nx < 0 || nx >= c || ny < 0 || ny >= l) continue;
            if (!isupper(tab[ny][nx]))               continue;

            int idx = ny * c + nx;
            if (vis[idx])                            continue;

            vis[idx] = true;
            qx[tail] = nx;
            qy[tail] = ny;
            tail++;
        }
    }

    free(vis);
    free(qx);
    free(qy);
    return visit == total;
}

/* '#' tem de estar rodeado apenas por brancas */
int vizinhosSaoBrancos(char **tab, int l, int c, Coordenadas p) {
    const int dx[4] = {1, -1, 0, 0},
              dy[4] = {0, 0, 1, -1};

    for (int k = 0; k < 4; ++k) {
        int nx = p.x + dx[k], ny = p.y + dy[k];
        if (nx < 0 || nx >= c || ny < 0 || ny >= l) continue;
        if (!isupper(tab[ny][nx])) return 0; /* vizinho não é branco */
    }
    return 1;
}

/*duplicados, # mal vizinho, minúsculas */
int regrasBasicas(char **tab, int l, int c,
                         int *dup, int *hash, int *minus) {
    int linha[l][26];
    int coluna[c][26];
    memset(linha, 0, sizeof linha);
    memset(coluna, 0, sizeof coluna);

    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (isupper(tab[y][x])) {
                int id = tab[y][x] - 'A';
                linha [y][id]++;
                coluna[x][id]++;
            }

    *dup = *hash = *minus = 0;

    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x) {
            char ch = tab[y][x];

            if (isupper(ch) && (linha[y][ch - 'A'] > 1 || coluna[x][ch - 'A'] > 1))
                *dup = 1;
            else if (ch == '#' && !vizinhosSaoBrancos(tab, l, c, (Coordenadas){x, y}))
                *hash = 1;
            else if (islower(ch) && (linha[y][ch - 'a'] || coluna[x][ch - 'a']))
                *minus = 1;
        }

    return !(*dup || *hash || *minus);
}

int regrasBasicasOk(char **tab, int l, int c) {
    int d, h, m;
    return regrasBasicas(tab, l, c, &d, &h, &m);
}

int verificaEstado(char **tab, int l, int c) {
    int dup, hash, minus;
    int okBasicas    = regrasBasicas(tab, l, c, &dup, &hash, &minus);
    int erroLigacao  = !casasBrancasConectadas(tab, l, c);

    if (okBasicas && !erroLigacao) {
        puts("Estado válido - nenhuma restrição violada.");
        return 1;
    }

    puts("Jogo inválido:");
    if (dup)         puts("- Há letras brancas repetidas na mesma linha/coluna");
    if (hash)        puts("- Existem casas '#' com vizinhos que não estão brancos");
    if (minus)       puts("- Há letras minúsculas que deviam estar riscadas (#)");
    if (erroLigacao) puts("- Nem todas as casas brancas estão conectadas entre si");
    return 0;
}
