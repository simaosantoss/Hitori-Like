#include <CUnit/Basic.h>
#include <stdlib.h>
#include <ctype.h>
#include "tabuleiro.h"

// Função auxiliar para preencher o tabuleiro com dados fixos para os testes
void preencherTabuleiroTeste(char **tabuleiro, int linhas, int colunas) {
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

// Teste da função de criação do tabuleiro
void testCriaTabuleiro(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    CU_ASSERT_PTR_NOT_NULL(tab);
    for (int i = 0; i < linhas; i++)
        CU_ASSERT_PTR_NOT_NULL(tab[i]);
    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

// Teste para pintura de casas válidas
void testPintaBrancoValido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    Coordenadas c = {0, 0}; // 'e'
    int res = pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[0][0], 'E');

    Coordenadas c2 = {3, 1}; // 'e'
    res = pintaBranco(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[1][3], 'E');

    Coordenadas c3 = {4, 2}; // 'c'
    res = pintaBranco(tab, linhas, colunas, c3);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[2][4], 'E');

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

// Teste para falhas ao pintar casas inválidas
void testPintaBrancoInvalido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    Coordenadas c = {1, 0}; // 'c'
    int res = pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[0][1], 'C');

    // Tentativa de pintar novamente
    res = pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 0);

    // Riscando célula
    Coordenadas c2 = {2, 2}; // 'd'
    res = pintaBranco(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 1);

    Coordenadas v1 = {2, 1}, v2 = {2, 3}, v3 = {1, 2}, v4 = {3, 2};
    pintaBranco(tab, linhas, colunas, v1);
    pintaBranco(tab, linhas, colunas, v2);
    pintaBranco(tab, linhas, colunas, v3);
    pintaBranco(tab, linhas, colunas, v4);

    res = riscaQuadrado(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 1);

    // Tentar pintar uma célula riscada
    res = pintaBranco(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(tab[2][2], '#');

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

// Teste de riscar célula com vizinhos corretos
void testRiscaQuadradoValido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    Coordenadas v1 = {2, 1}, v2 = {2, 3}, v3 = {1, 2}, v4 = {3, 2};
    pintaBranco(tab, linhas, colunas, v1);
    pintaBranco(tab, linhas, colunas, v2);
    pintaBranco(tab, linhas, colunas, v3);
    pintaBranco(tab, linhas, colunas, v4);

    Coordenadas alvo = {2, 2};
    int res = riscaQuadrado(tab, linhas, colunas, alvo);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[2][2], '#');

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

// Teste de tentativa inválida de riscar
void testRiscaQuadradoInvalido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    Coordenadas c = {0, 0}; // 'e'
    pintaBranco(tab, linhas, colunas, c);
    int res = riscaQuadrado(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 0);

    Coordenadas c2 = {4, 4}; // sem vizinhos brancos
    res = riscaQuadrado(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 0);

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

// Teste de conversão de maiúscula para minúscula
void testConverteParaMinuscula(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    Coordenadas c = {1, 0}; // 'c'
    pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_TRUE(isupper(tab[0][1]));

    int res = converteParaMinuscula(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_TRUE(islower(tab[0][1]));

    Coordenadas c2 = {2, 2}; // não maiúscula
    res = converteParaMinuscula(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 0);

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

// Teste de coordenadas inválidas
void testCoordenadaInvalida(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    Coordenadas c = {-1, 0};
    CU_ASSERT_EQUAL(pintaBranco(tab, linhas, colunas, c), 0);
    CU_ASSERT_EQUAL(riscaQuadrado(tab, linhas, colunas, c), 0);

    Coordenadas c2 = {0, -1};
    CU_ASSERT_EQUAL(pintaBranco(tab, linhas, colunas, c2), 0);
    CU_ASSERT_EQUAL(riscaQuadrado(tab, linhas, colunas, c2), 0);

    Coordenadas c3 = {5, 5};
    CU_ASSERT_EQUAL(pintaBranco(tab, linhas, colunas, c3), 0);
    CU_ASSERT_EQUAL(riscaQuadrado(tab, linhas, colunas, c3), 0);

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

// Função principal com os testes registrados
int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Suite_Tabuleiro", 0, 0);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Criação do Tabuleiro", testCriaTabuleiro);
    CU_add_test(suite, "Pintura Válida", testPintaBrancoValido);
    CU_add_test(suite, "Pintura Inválida", testPintaBrancoInvalido);
    CU_add_test(suite, "Riscar Válido", testRiscaQuadradoValido);
    CU_add_test(suite, "Riscar Inválido", testRiscaQuadradoInvalido);
    CU_add_test(suite, "Conversão para Minúscula", testConverteParaMinuscula);
    CU_add_test(suite, "Coordenadas Inválidas", testCoordenadaInvalida);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
