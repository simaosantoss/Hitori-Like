#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tabuleiro.h"

char **criaTabuleiro(int linhas,int colunas)
{
    char **t = malloc(linhas * sizeof *t);
    for (int i=0;i<linhas;++i) t[i] = malloc(colunas * sizeof **t);
    return t;
}

void recebeTabuleiro(char **t,int l,int c)
{
    for (int y=0;y<l;++y)
        for (int x=0;x<c;++x)
            scanf(" %c",&t[y][x]);
}

void imprimeTabuleiro(char **t,int l,int c)
{
    for (int y=0;y<l;++y){
        for (int x=0;x<c;++x) printf("%c ",t[y][x]);
        puts("");
    }
}

static int vizinhosSaoBrancos(char **tab,int l,int c,Coordenadas p)
{
    if (p.y-1>=0 && !isupper(tab[p.y-1][p.x])) return 0;
    if (p.y+1< l && !isupper(tab[p.y+1][p.x])) return 0;
    if (p.x-1>=0 && !isupper(tab[p.y][p.x-1])) return 0;
    if (p.x+1< c && !isupper(tab[p.y][p.x+1])) return 0;
    return 1;
}

int pintaBranco(char **tab,int l,int c,Coordenadas p)
{
    if (p.x<0||p.x>=c||p.y<0||p.y>=l){ printf("Coordenada fora do tabuleiro.\n");return 0;}
    char ch = tab[p.y][p.x];
    if (isupper(ch))   { puts("Erro: já está pintado de branco.");  return 0; }
    if (ch=='#')       { puts("Erro: não se pode pintar uma casa riscada."); return 0; }

    tab[p.y][p.x] = toupper(ch);
    return 1;
}

int riscaQuadrado(char **tab,int l,int c,Coordenadas p)
{
    if (p.x<0||p.x>=c||p.y<0||p.y>=l){ printf("Coordenada fora do tabuleiro.\n");return 0;}
    char ch = tab[p.y][p.x];
    if (ch=='#')      { puts("Erro: já está riscado."); return 0; }
    if (isupper(ch)){ puts("Erro: não se pode riscar uma casa pintada de branco (faça undo primeiro!).");return 0;}

    tab[p.y][p.x] = '#';
    return 1;
}

int converteParaMinuscula(char **tab,int l,int c,Coordenadas p)
{
    if (p.x<0||p.x>=c||p.y<0||p.y>=l){ printf("Coordenada fora do tabuleiro.\n");return 0;}
    if (!isupper(tab[p.y][p.x])){ puts("Erro: a casa não está em maiúscula, não faz sentido converter."); return 0; }

    tab[p.y][p.x] = tolower(tab[p.y][p.x]);
    return 1;
}

// comando v
int verificaEstado(char **t,int l,int c)
{
    int duplicados=0, hashErr=0, minusculasErr=0;

    int rowU[l][26]; int colU[c][26];
    memset(rowU,0,sizeof rowU);
    memset(colU,0,sizeof colU);

    for (int y=0;y<l;++y)
        for (int x=0;x<c;++x)
            if (isupper(t[y][x])){
                int id=t[y][x]-'A';
                rowU[y][id]++; colU[x][id]++;
            }

    for (int y=0;y<l;++y)
        for (int x=0;x<c;++x){
            char ch=t[y][x];
            if (isupper(ch) && (rowU[y][ch-'A']>1 || colU[x][ch-'A']>1))
                duplicados = 1;
            else if (ch=='#' && !vizinhosSaoBrancos(t,l,c,(Coordenadas){x,y}))
                hashErr = 1;
            else if (islower(ch) && (rowU[y][ch-'a'] || colU[x][ch-'a']))
                minusculasErr = 1;
        }

    if(!duplicados && !hashErr && !minusculasErr){
        puts("Estado válido - nenhuma restrição violada.");
        return 1;
    }

    puts("Jogo inválido:");
    if (duplicados)
        puts("- Há letras brancas repetidas na mesma linha/coluna");
    if (hashErr)
        puts("- Existem casas '#' com vizinhos que não estão brancos");
    if (minusculasErr)
        puts("- Há letras minúsculas que deviam estar riscadas (#)");

    return 0;
}
