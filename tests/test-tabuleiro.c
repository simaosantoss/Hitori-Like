#include <stdio.h> 
#include <stdlib.h>
#include <CUnit/Basic.h>
#include <ctype.h>
#include "tabuleiro.h"

void preencherTabuleiro(char **tabuleiro, int linhas, int colunas) {
    char init[5][5] = {
        {'e', 'c', 'a', 'd', 'c'},
        {'d', 'c', 'd', 'e', 'c'},
        {'b', 'd', 'd', 'c', 'e'},
        {'c', 'd', 'e', 'e', 'b'},
        {'a', 'c', 'c', 'b', 'b'}
    };
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            tabuleiro[i][j] = init[i][j];
}

void test_criaTabuleiro(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    CU_ASSERT_PTR_NOT_NULL(tab);
    for (int i = 0; i < linhas; i++)
        CU_ASSERT_PTR_NOT_NULL(tab[i]);
    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

void test_pintaBranco(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiro(tab, linhas, colunas);
    
    Coordenadas ponto = {0, 0};
    pintaBranco(tab, linhas, colunas, ponto);
    CU_ASSERT_EQUAL(tab[0][0], 'E');

    Coordenadas outro = {3, 1};
    pintaBranco(tab, linhas, colunas, outro);
    CU_ASSERT_EQUAL(tab[1][3], 'E');

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

void test_pintaBrancoInvalido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiro(tab, linhas, colunas);

    Coordenadas ponto = {0, 0};
    pintaBranco(tab, linhas, colunas, ponto); // primeira vez → válido
    char antes = tab[0][0];

    pintaBranco(tab, linhas, colunas, ponto); // segunda vez → inválido
    CU_ASSERT_EQUAL(tab[0][0], antes); // deve continuar igual (E)

    Coordenadas riscada = {1, 0};
    riscaQuadrado(tab, linhas, colunas, riscada); // risca
    pintaBranco(tab, linhas, colunas, riscada); // tentar pintar riscado
    CU_ASSERT_EQUAL(tab[0][1], '#'); // deve continuar riscado

    for (int i = 0; i < linhas; i++) free(tab[i]);
    free(tab);
}

void test_riscaQuadrado(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiro(tab, linhas, colunas);
    
    Coordenadas ponto = {2, 2};
    riscaQuadrado(tab, linhas, colunas, ponto);
    CU_ASSERT_EQUAL(tab[2][2], '#');

    Coordenadas outro = {1, 0};
    riscaQuadrado(tab, linhas, colunas, outro);
    CU_ASSERT_EQUAL(tab[0][1], '#');

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

void test_riscaQuadradoInvalido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiro(tab, linhas, colunas);

    Coordenadas ponto = {2, 2};
    riscaQuadrado(tab, linhas, colunas, ponto); // primeiro riscado
    char antes = tab[2][2];

    riscaQuadrado(tab, linhas, colunas, ponto); // segunda tentativa → inválida
    CU_ASSERT_EQUAL(tab[2][2], antes);

    Coordenadas pintado = {0, 0};
    pintaBranco(tab, linhas, colunas, pintado); // pintar de branco
    riscaQuadrado(tab, linhas, colunas, pintado); // tentar riscar branco
    CU_ASSERT_EQUAL(tab[0][0], 'E'); // deve manter-se branco

    for (int i = 0; i < linhas; i++) free(tab[i]);
    free(tab);
}

void test_coordenadaInvalida(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiro(tab, linhas, colunas);

    char original = tab[0][0];

    Coordenadas invalida = {0, 6}; // linha inválida
    pintaBranco(tab, linhas, colunas, invalida);
    CU_ASSERT_EQUAL(tab[0][0], original);

    riscaQuadrado(tab, linhas, colunas, invalida);
    CU_ASSERT_EQUAL(tab[0][0], original);

    for (int i = 0; i < linhas; i++) free(tab[i]);
    free(tab);
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Suite Etapa1", 0, 0);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (!CU_add_test(suite, "Teste criaTabuleiro", test_criaTabuleiro) ||
        !CU_add_test(suite, "Teste pintaBranco", test_pintaBranco) ||
        !CU_add_test(suite, "Teste pintaBranco inválido", test_pintaBrancoInvalido) ||
        !CU_add_test(suite, "Teste riscaQuadrado", test_riscaQuadrado) ||
        !CU_add_test(suite, "Teste riscaQuadrado inválido", test_riscaQuadradoInvalido) ||
        !CU_add_test(suite, "Teste coordenadas inválidas", test_coordenadaInvalida)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
