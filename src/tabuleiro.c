#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>          
#include "tabuleiro.h"


/* adicionei estas caixas todas pipis para ser mais fácil para voces conseguirem perceber o que meti aqui e conseguir
dividir o vosso pensamento por partes */



char **criaTabuleiro(int linhas,int colunas)
{
    char **tabuleiro = malloc(linhas * sizeof *tabuleiro);
    for (int i=0;i<linhas;++i) tabuleiro[i] = malloc(colunas * sizeof **tabuleiro);
    return tabuleiro;
}

void recebeTabuleiro(char **tabuleiro,int linhas,int colunas)
{
    for (int y=0;y<linhas;++y)
        for (int x=0;x<colunas;++x)
            scanf(" %c",&tabuleiro[y][x]);
}

void imprimeTabuleiro(char **tabuleiro,int linhas,int colunas)
{
    for (int y=0;y<linhas;++y){
        for (int x=0;x<colunas;++x) printf("%c ",tabuleiro[y][x]);
        puts("");
    }
}

// ============================================================= 

/* devolve 1 se todos os 4 vizinhos ortogonais (quando existem)
   forem maiúsculas; caso contrário devolve 0 */
static int vizinhosSaoBrancos(char **tabuleiro,int linhas,int colunas,Coordenadas p)
{
    if (p.y-1>=0 && !isupper(tabuleiro[p.y-1][p.x])) return 0;
    if (p.y+1< linhas && !isupper(tabuleiro[p.y+1][p.x])) return 0;
    if (p.x-1>=0 && !isupper(tabuleiro[p.y][p.x-1])) return 0;
    if (p.x+1< colunas && !isupper(tabuleiro[p.y][p.x+1])) return 0;
    return 1;
}


int pintaBranco(char **tabuleiro,int linhas,int colunas,Coordenadas p)
{
    if (p.x<0||p.x>=colunas||p.y<0||p.y>=linhas){ printf("Coordenada fora do tabuleiro.\n");return 0;}
    char letra = tabuleiro[p.y][p.x];
    if (isupper(letra))   { puts("Erro: já está pintado de branco.");  return 0; }
    if (letra=='#')       { puts("Erro: não se pode pintar uma casa riscada."); return 0; }

    tabuleiro[p.y][p.x] = toupper(letra);
    return 1;
}

int riscaQuadrado(char **tabuleiro,int linhas,int colunas,Coordenadas p)
{
    if (p.x<0||p.x>=colunas||p.y<0||p.y>=linhas){ printf("Coordenada fora do tabuleiro.\n");return 0;}
    char letra = tabuleiro[p.y][p.x];
    if (letra=='#')      { puts("Erro: já está riscado."); return 0; }
    if (isupper(letra)){ puts("Erro: não se pode riscar uma casa pintada de branco (faça undo primeiro!).");return 0;}

    tabuleiro[p.y][p.x] = '#';
    return 1;
}

int converteParaMinuscula(char **tabuleiro,int linhas,int colunas,Coordenadas p)
{
    if (p.x<0||p.x>=colunas||p.y<0||p.y>=linhas){ printf("Coordenada fora do tabuleiro.\n");return 0;}
    if (!isupper(tabuleiro[p.y][p.x])){ puts("Erro: a casa não está em maiúscula, não faz sentido converter."); return 0; }

    tabuleiro[p.y][p.x] = tolower(tabuleiro[p.y][p.x]);
    return 1;
}

// Conectividade das casas brancas (v)

static int casasBrancasConectadas(char **tabuleiro,int linhas,int colunas)
{
    int totalBrancas = 0;
    int inicioX = -1, inicioY = -1;

    for (int y = 0; y < linhas; ++y)
        for (int x = 0; x < colunas; ++x)
            if (isupper(tabuleiro[y][x])){
                totalBrancas++;
                if (inicioX == -1){ inicioX = x; inicioY = y; }
            }

    if (totalBrancas == 0) return 1;

    bool *visitado = calloc(linhas * colunas, sizeof *visitado);     
    int  *filaX    = malloc(linhas * colunas * sizeof *filaX);          
    int  *filaY    = malloc(linhas * colunas * sizeof *filaY);
    int cabeca = 0, cauda = 0;

    filaX[cauda] = inicioX; filaY[cauda] = inicioY; cauda++;
    visitado[inicioY * colunas + inicioX] = true;

    int brancasVisitadas = 0;
    const int deslocX[4] = {1,-1,0,0};
    const int deslocY[4] = {0,0,1,-1};

    while (cabeca < cauda){
        int x = filaX[cabeca];
        int y = filaY[cabeca];
        cabeca++;
        brancasVisitadas++;

        for (int i = 0; i < 4; ++i){
            int novoX = x + deslocX[i], novoY = y + deslocY[i];
            if (novoX<0 || novoX>=colunas || novoY<0 || novoY>=linhas) continue;
            if (!isupper(tabuleiro[novoY][novoX])) continue;
            int idx = novoY * colunas + novoX;
            if (visitado[idx]) continue;

            visitado[idx] = true;
            filaX[cauda] = novoX; filaY[cauda] = novoY; cauda++;
        }
    }

    free(filaX); free(filaY); free(visitado);
    return brancasVisitadas == totalBrancas;
}

int verificaEstado(char **tabuleiro,int linhas,int colunas)
{
    int haDuplicados=0, haErrosHash=0, haErrosMinusculas=0;

    int letrasLinha[linhas][26]; 
    int letrasColuna[colunas][26];
    memset(letrasLinha,0,sizeof letrasLinha);
    memset(letrasColuna,0,sizeof letrasColuna);

    for (int y=0;y<linhas;++y)
        for (int x=0;x<colunas;++x)
            if (isupper(tabuleiro[y][x])){
                int id=tabuleiro[y][x]-'A';
                letrasLinha[y][id]++; 
                letrasColuna[x][id]++;
            }

    for (int y=0;y<linhas;++y)
        for (int x=0;x<colunas;++x){
            char ch=tabuleiro[y][x];
            if (isupper(ch) && (letrasLinha[y][ch-'A']>1 || letrasColuna[x][ch-'A']>1))
                haDuplicados = 1;
            else if (ch=='#' && !vizinhosSaoBrancos(tabuleiro,linhas,colunas,(Coordenadas){x,y}))
                haErrosHash = 1;
            else if (islower(ch) && (letrasLinha[y][ch-'a'] || letrasColuna[x][ch-'a']))
                haErrosMinusculas = 1;
        }

    int haErroConexao = !casasBrancasConectadas(tabuleiro,linhas,colunas);

    if(!haDuplicados && !haErrosHash && !haErrosMinusculas && !haErroConexao){
        puts("Estado válido - nenhuma restrição violada.");
        return 1;
    }

    puts("Jogo inválido:");
    if (haDuplicados)
        puts("- Há letras brancas repetidas na mesma linha/coluna");
    if (haErrosHash)
        puts("- Existem casas '#' com vizinhos que não estão brancos");
    if (haErrosMinusculas)
        puts("- Há letras minúsculas que deviam estar riscadas (#)");
    if (haErroConexao)
        puts("- Nem todas as casas brancas estão conectadas entre si");

    return 0;
}
