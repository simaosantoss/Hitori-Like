#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include "interface.h"
#include "ficheiros.h"
#include "stack.h"

/* ---------- registar alterações em bloco ---------- */
static void registaMudancas(StackMovimentos *st,
                            char **antes,char **depois,int l,int c)
{
    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            if (antes[y][x] != depois[y][x])
                push(st, (Movimento){x, y, antes[y][x], depois[y][x]});
}


void lerComando(char ***ptab,int *l,int *c,StackMovimentos *stack)
{
    char cmd[64], letra; int num;

    while(1){
        imprimeTabuleiro(*ptab,*l,*c);
        puts("Comandos: b/r/f <coord>, a, A, R, v, d, g/l <fic>, s");
        if(scanf(" %s",cmd)!=1) continue;

        /* ---------- sair ---------- */
        if(strcmp(cmd,"s")==0){ puts("Até breve!"); break; }

        /* ---------- comando a ---------- */
        else if(strcmp(cmd,"a")==0){
            char **antes = duplicaTabuleiro(*ptab,*l,*c);
            int mudou   = aplica_comando_a(*ptab,*l,*c);
            if(mudou){
                registaMudancas(stack, antes, *ptab, *l, *c);
            } else
                puts("[a] nada para propagar.");
            for(int i=0;i<*l;i++) free(antes[i]); free(antes);
        }

        /* ---------- comando A ---------- */
        else if(strcmp(cmd,"A")==0){
            char **antes = duplicaTabuleiro(*ptab,*l,*c);
            int it      = aplica_comando_A(*ptab,*l,*c);
            if(it){
                registaMudancas(stack, antes, *ptab, *l, *c);
                printf("[A] %d iteração(ões) executadas.\n",it);
            } else
                puts("[A] nada para propagar.");
            for(int i=0;i<*l;i++) free(antes[i]); free(antes);
        }
        /* ---------- comando R ---------- */
        else if(strcmp(cmd,"R")==0){
            char **tmp=duplicaTabuleiro(*ptab,*l,*c);
            if(resolverJogo(tmp,*l,*c)){
                copiaTabuleiro(*ptab,tmp,*l,*c);
                puts("Puzzle resolvido! 🙂");
            }else
                puts("Não foi possível resolver 🤔");
            for(int i=0;i<*l;++i) free(tmp[i]); free(tmp);
        }

        /* ---------- pintar de branco ---------- */
        else if(strcmp(cmd,"b")==0){
            if(scanf(" %c%d",&letra,&num)!=2){ puts("Coord inválida."); continue; }
            Coordenadas p={letra-'a',num-1};
            char ant=(*ptab)[p.y][p.x];
            if(pintaBranco(*ptab,*l,*c,p)){
                push(stack,(Movimento){p.x,p.y,ant,(*ptab)[p.y][p.x]});
            }
        }
        /* ---------- riscar ---------- */
        else if(strcmp(cmd,"r")==0){
            if(scanf(" %c%d",&letra,&num)!=2){ puts("Coord inválida."); continue; }
            Coordenadas p={letra-'a',num-1};
            char ant=(*ptab)[p.y][p.x];
            if(riscaQuadrado(*ptab,*l,*c,p)){
                push(stack,(Movimento){p.x,p.y,ant,(*ptab)[p.y][p.x]});
            }
        }
        /* ---------- forçar minúscula ---------- */
        else if(strcmp(cmd,"f")==0){
            if(scanf(" %c%d",&letra,&num)!=2){ puts("Coord inválida."); continue; }
            Coordenadas p={letra-'a',num-1};
            char ant=(*ptab)[p.y][p.x];
            if(converteParaMinuscula(*ptab,*l,*c,p)){
                push(stack,(Movimento){p.x,p.y,ant,(*ptab)[p.y][p.x]});
            }
        }
        /* ---------- desfazer ---------- */
        else if(strcmp(cmd,"d")==0){
            Movimento m;
            if(pop(stack,&m)) (*ptab)[m.y][m.x]=m.valorAntigo;
            else puts("Nada a desfazer.");
        }
        /* ---------- gravar / ler ---------- */
        else if(strcmp(cmd,"g")==0){
            char nome[64]; if(scanf(" %s",nome)!=1) continue;
            gravarTabuleiroFicheiro(nome,*ptab,*l,*c);
        }
        else if(strcmp(cmd,"l")==0){
            char nome[64]; if(scanf(" %s",nome)!=1) continue;
            libertaMemoria(*ptab,*l);
            *ptab=lerTabuleiroFicheiro(nome,l,c);
            while(pop(stack,&(Movimento){0}));
        }
        /* ---------- verificar ---------- */
        else if(strcmp(cmd,"v")==0){
            verificaEstado(*ptab,*l,*c);
        }

        /* ---------- comando desconhecido ---------- */
        else puts("Comando desconhecido.");
    }
}
