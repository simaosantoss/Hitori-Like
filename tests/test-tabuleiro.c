#include <CUnit/Basic.h>
#include <stdlib.h>
#include <ctype.h>
#include "tabuleiro.h"

static void preencherTabuleiroTeste(char **t, int l, int c) {
    char init[5][5] = {
        {'e','c','a','d','c'},
        {'d','c','d','e','c'},
        {'b','d','d','c','e'},
        {'c','d','e','e','b'},
        {'a','c','c','b','b'}
    };
    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            t[y][x] = init[y][x];
}

void testCriaTabuleiro(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c);
    CU_ASSERT_PTR_NOT_NULL(tab);
    for (int i = 0; i < l; ++i) CU_ASSERT_PTR_NOT_NULL(tab[i]);
    for (int i = 0; i < l; ++i) free(tab[i]);
    free(tab);
}


void testPintaBrancoValido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); preencherTabuleiroTeste(tab, l, c);

    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, (Coordenadas){0,0}), 1);
    CU_ASSERT_EQUAL(tab[0][0], 'E');
    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, (Coordenadas){3,1}), 1);
    CU_ASSERT_EQUAL(tab[1][3], 'E');
    for (int i = 0; i < l; ++i) free(tab[i]); free(tab);
}

void testPintaBrancoInvalido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); preencherTabuleiroTeste(tab, l, c);

    Coordenadas c1 = {1,0};
    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, c1), 1);
    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, c1), 0);           
    for (int i = 0; i < l; ++i) free(tab[i]); free(tab);
}


void testRiscaQuadradoValido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); preencherTabuleiroTeste(tab, l, c);

    Coordenadas v[4] = {{2,1},{2,3},{1,2},{3,2}};
    for (int i = 0; i < 4; ++i) pintaBranco(tab, l, c, v[i]);

    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, (Coordenadas){2,2}), 1);
    CU_ASSERT_EQUAL(tab[2][2], '#');
    for (int i = 0; i < l; ++i) free(tab[i]); free(tab);
}

void testRiscaQuadradoInvalido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); preencherTabuleiroTeste(tab, l, c);

    pintaBranco(tab, l, c, (Coordenadas){0,0});
    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, (Coordenadas){0,0}), 0); 
    for (int i = 0; i < l; ++i) free(tab[i]); free(tab);
}

void testConverteParaMinuscula(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); preencherTabuleiroTeste(tab, l, c);

    Coordenadas p = {1,0};
    pintaBranco(tab, l, c, p);
    CU_ASSERT_EQUAL(converteParaMinuscula(tab, l, c, p), 1);
    CU_ASSERT_TRUE(islower(tab[0][1]));
    for (int i = 0; i < l; ++i) free(tab[i]); free(tab);
}

void testCoordenadaInvalida(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); preencherTabuleiroTeste(tab, l, c);

    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, (Coordenadas){-1,0}), 0);
    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, (Coordenadas){5,5}), 0);
    for (int i = 0; i < l; ++i) free(tab[i]); free(tab);
}



void testConectividadeValida(void){
    int l=2,c=2; char **tab=criaTabuleiro(l,c);
    tab[0][0]='A'; tab[0][1]='B';
    tab[1][0]='c'; tab[1][1]='d';
    CU_ASSERT_EQUAL(verificaEstado(tab,l,c),1);
    for(int i=0;i<l;++i) free(tab[i]); free(tab);
}


void testConectividadeInvalida(void) {
    int l = 3, c = 3;
    char **tab = criaTabuleiro(l, c);

    for (int y = 0; y < l; ++y)
        for (int x = 0; x < c; ++x)
            tab[y][x] = 'c';
    tab[0][0] = 'A';
    tab[2][2] = 'B';
    CU_ASSERT_EQUAL(verificaEstado(tab, l, c), 0);
    for (int i = 0; i < l; ++i) free(tab[i]); free(tab);
}


int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();

    CU_pSuite s = CU_add_suite("Suite_Tabuleiro", 0, 0);
    if (!s) { CU_cleanup_registry(); return CU_get_error(); }


    CU_add_test(s, "Criação do Tabuleiro",           testCriaTabuleiro);
    CU_add_test(s, "Pintura Válida",                 testPintaBrancoValido);
    CU_add_test(s, "Pintura Inválida",               testPintaBrancoInvalido);
    CU_add_test(s, "Riscar Válido",                  testRiscaQuadradoValido);
    CU_add_test(s, "Riscar Inválido",                testRiscaQuadradoInvalido);
    CU_add_test(s, "Conversão para Minúscula",       testConverteParaMinuscula);
    CU_add_test(s, "Coordenadas Inválidas",          testCoordenadaInvalida);

    CU_add_test(s, "Conectividade Válida",           testConectividadeValida);
    CU_add_test(s, "Conectividade Inválida",         testConectividadeInvalida);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
