#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "../include/tabuleiro.h"

// criação do tabuleiro 
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

// primitivas de edição 
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

// Implementação do comando a 
static int risca_replicas(char **tab, int l, int c) {
    int mudou = 0;
    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (isupper(tab[y][x])) {
                char target = tolower(tab[y][x]);
                // linha 
                for (int xx = 0; xx < c; ++xx)
                    if (islower(tab[y][xx]) && tab[y][xx] == target)
                        tab[y][xx] = '#', mudou = 1;
                // coluna 
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

// auxiliares para o comando R 
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

// verifica, nos brancos atuais, se ainda é possível conectá-los através de células que não sejam '#'. Se houver duas partes completamente isoladas por '#', retorna 0. 
static int conexao_possivel(char **tab, int l, int c) {
    // 1) Conta quantas maiúsculas há e guarda a posição da primeira
    int totalBrancas = 0;
    int sx = -1, sy = -1;
    for (int y = 0; y < l; ++y) {
        for (int x = 0; x < c; ++x) {
            if (isupper(tab[y][x])) {
                totalBrancas++;
                if (sx < 0) {
                    sx = x;
                    sy = y;
                }
            }
        }
    }
    if (totalBrancas <= 1) {
        // Ou não há brancas, ou só há uma: está sempre “conectável”
        return 1;
    }

    // 2) BFS que começa em (sy, sx), mas navega por TODAS as células != '#'
    bool *vis = calloc(l * c, sizeof(bool));
    int *qx  = malloc(l * c * sizeof(int));
    int *qy  = malloc(l * c * sizeof(int));
    int head = 0, tail = 0;

    qx[tail] = sx;
    qy[tail] = sy;
    tail++;
    vis[sy * c + sx] = true;

    int brancasAlcancadas = 0;
    const int dx[4] = {1, -1, 0, 0};
    const int dy[4] = {0, 0, 1, -1};

    while (head < tail) {
        int x = qx[head];
        int y = qy[head];
        head++;
        if (isupper(tab[y][x])) {
            brancasAlcancadas++;
        }
        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k];
            int ny = y + dy[k];
            if (nx < 0 || nx >= c || ny < 0 || ny >= l) continue;
            if (tab[ny][nx] == '#')      continue; // não podemos atravessar casas riscadas
            int idx = ny * c + nx;
            if (vis[idx])               continue;
            vis[idx] = true;
            qx[tail] = nx;
            qy[tail] = ny;
            tail++;
        }
    }

    free(vis);
    free(qx);
    free(qy);

    // Se o número de brancas alcançadas for menor que o total de brancas, há um corte irremediável.
    return (brancasAlcancadas == totalBrancas);
}

// backtracking (comando R) (vou meter por passos para perceberem o que fiz)
int resolverJogo(char **tab, int l, int c) {
    // 1ª propagação
    aplica_comando_A(tab, l, c);

    // 1.a) Testa regras básicas; se falhar, descartar
    if (!regrasBasicasOk(tab, l, c))
        return 0;

    // 1.b) testa se as brancas atuais ainda podem ficar todas conectadas
    if (!conexao_possivel(tab, l, c))
        return 0;

    // 2) Se não há mais minúsculas, estamos num “estado terminal”:
    if (!tem_minusculas(tab, l, c)) {
        // Usar verificaEstado para confirmar ligação final
        return verificaEstado(tab, l, c);
    }

    // 3) Há minúsculas -> escolhe uma célula para ramificar
    Coordenadas cel = escolhe_celula(tab, l, c);

    // 3.a) tentar “branco” nessa célula
    {
        char **tmp = duplicaTabuleiro(tab, l, c);
        pintaBranco(tmp, l, c, cel);
        if (resolverJogo(tmp, l, c)) {
            // se este ramo encontrar solução, copia resultado para 'tab' e retorna 1
            copiaTabuleiro(tab, tmp, l, c);
            for (int i = 0; i < l; ++i) free(tmp[i]);
            free(tmp);
            return 1;
        }
        // senão, libera tmp e tenta próximo ramo
        for (int i = 0; i < l; ++i) free(tmp[i]);
        free(tmp);
    }

    // 3.b) tentar “#” nessa célula
    {
        char **tmp = duplicaTabuleiro(tab, l, c);
        riscaQuadrado(tmp, l, c, cel);
        if (resolverJogo(tmp, l, c)) {
            copiaTabuleiro(tab, tmp, l, c);
            for (int i = 0; i < l; ++i) free(tmp[i]);
            free(tmp);
            return 1;
        }
        for (int i = 0; i < l; ++i) free(tmp[i]);
        free(tmp);
    }

    // nenhum ramo deu certo -> sem solução
    return 0;
}
