#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/tabuleiro.h"
#include "../include/interface.h"
#include "../include/ficheiros.h"
#include "../include/stack.h"

//  cópia original do puzzle 
static char **tabOriginal = NULL;
static int    lOrig = 0, cOrig = 0;

void guardaOriginal(char **tab, int l, int c) {
    // Liberta eventual cópia anterior 
    if (tabOriginal) {
        for (int i = 0; i < lOrig; ++i)
            free(tabOriginal[i]);
        free(tabOriginal);
    }
    tabOriginal = duplicaTabuleiro(tab, l, c);
    lOrig       = l;
    cOrig       = c;
}

//  utilitário interno  
static void registaMudancas(StackMovimentos *st,
                            char **antes, char **depois,
                            int l, int c) {
    Movimento marcador = { -1, -1, 0, 0 };
    push(st, marcador); // marcador do início do comando 

    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (antes[y][x] != depois[y][x])
                push(st, (Movimento){ x, y, antes[y][x], depois[y][x] });
}

void lerComando(char ***ptab, int *l, int *c, StackMovimentos *stack) {
    char cmd[64], letra;
    int  num;

    while (1) {
        imprimeTabuleiro(*ptab, *l, *c);
        puts("Comandos: b/r/f <coord>, a, A, R, v, d, g/l <fic>, s");
        if (scanf(" %s", cmd) != 1) continue;

        // sair 
        if (strcmp(cmd, "s") == 0) {
            puts("Até breve!");
            break;
        }

        // comando a 
        else if (strcmp(cmd, "a") == 0) {
            char **antes = duplicaTabuleiro(*ptab, *l, *c);
            if (aplica_comando_a(*ptab, *l, *c))
                registaMudancas(stack, antes, *ptab, *l, *c);
            else
                puts("[a] nada para propagar.");
            libertaMemoria(antes, *l);
        }

        // comando A 
        else if (strcmp(cmd, "A") == 0) {
            char **antes = duplicaTabuleiro(*ptab, *l, *c);
            int    it    = aplica_comando_A(*ptab, *l, *c);
            if (it) {
                registaMudancas(stack, antes, *ptab, *l, *c);
                printf("[A] %d iteração(ões) executadas.\n", it);
            } else
                puts("[A] nada para propagar.");
            libertaMemoria(antes, *l);
        }

        // comando R 
        else if (strcmp(cmd, "R") == 0) {
            // 1) escolher ponto de partida (original se existir) 
            char **tmp = tabOriginal
                           ? duplicaTabuleiro(tabOriginal, lOrig, cOrig)
                           : duplicaTabuleiro(*ptab, *l, *c);

            // 2) tentar resolver 
            int ok = resolverJogo(tmp,
                                  tabOriginal ? lOrig : *l,
                                  tabOriginal ? cOrig : *c);

            if (ok) {
                char **antes = duplicaTabuleiro(*ptab, *l, *c);
                // Garante as mesmas dimensões do que o tabuleiro em uso 
                copiaTabuleiro(*ptab, tmp, *l, *c);
                registaMudancas(stack, antes, *ptab, *l, *c);
                libertaMemoria(antes, *l);
                puts("Puzzle resolvido! 🙂");
            } else
                puts("Não foi possível resolver 🤔");

            libertaMemoria(tmp, tabOriginal ? lOrig : *l);
        }

        // comando b (pintar branco) 
        else if (strcmp(cmd, "b") == 0) {
            if (scanf(" %c%d", &letra, &num) != 2) {
                puts("Coord inválida.");
                continue;
            }
            Coordenadas p = { letra - 'a', num - 1 };
            if (p.x < 0 || p.x >= *c || p.y < 0 || p.y >= *l) {
                puts("Coordenadas fora do tabuleiro.");
                continue;
            }
            char ant = (*ptab)[p.y][p.x];
            if (pintaBranco(*ptab, *l, *c, p))
                push(stack, (Movimento){ p.x, p.y, ant, (*ptab)[p.y][p.x] });
        }

        // comando r (riscar) 
        else if (strcmp(cmd, "r") == 0) {
            if (scanf(" %c%d", &letra, &num) != 2) {
                puts("Coord inválida.");
                continue;
            }
            Coordenadas p = { letra - 'a', num - 1 };
            if (p.x < 0 || p.x >= *c || p.y < 0 || p.y >= *l) {
                puts("Coordenadas fora do tabuleiro.");
                continue;
            }
            char ant = (*ptab)[p.y][p.x];
            if (riscaQuadrado(*ptab, *l, *c, p))
                push(stack, (Movimento){ p.x, p.y, ant, (*ptab)[p.y][p.x] });
        }

        // comando f (forçar minúscula) 
        else if (strcmp(cmd, "f") == 0) {
            if (scanf(" %c%d", &letra, &num) != 2) {
                puts("Coord inválida.");
                continue;
            }
            Coordenadas p = { letra - 'a', num - 1 };
            if (p.x < 0 || p.x >= *c || p.y < 0 || p.y >= *l) {
                puts("Coordenadas fora do tabuleiro.");
                continue;
            }
            char ant = (*ptab)[p.y][p.x];
            if (converteParaMinuscula(*ptab, p))
                push(stack, (Movimento){ p.x, p.y, ant, (*ptab)[p.y][p.x] });
        }

        // desfazer 
        else if (strcmp(cmd, "d") == 0) {
            Movimento m;
            if (!pop(stack, &m)) {
                puts("Nada a desfazer.");
                continue;
            }
            // desfaz até ao marcador 
            while (!(m.x == -1 && m.y == -1)) {
                (*ptab)[m.y][m.x] = m.valorAntigo;
                if (!pop(stack, &m)) break; // segurança 
            }
        }

        // gravar / ler
        else if (strcmp(cmd, "g") == 0) {
            char nome[64];
            if (scanf(" %s", nome) != 1) continue;
            gravarTabuleiroFicheiro(nome, *ptab, *l, *c);
        } else if (strcmp(cmd, "l") == 0) {
            char nome[64];
            if (scanf(" %s", nome) != 1) continue;
            libertaMemoria(*ptab, *l);
            *ptab = lerTabuleiroFicheiro(nome, l, c);
            while (pop(stack, &(Movimento){0})); // limpa undo 
            guardaOriginal(*ptab, *l, *c);       // nova cópia original 
        }

        // verificar
        else if (strcmp(cmd, "v") == 0) {
            verificaEstado(*ptab, *l, *c);
        }

        // comando desconhecido 
        else {
            puts("Comando desconhecido.");
        }
    }
}
