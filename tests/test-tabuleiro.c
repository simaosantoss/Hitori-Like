#include <stdio.h> 
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "tabuleiro.h"

// Função auxiliar para preencher o tabuleiro com os valores iniciais do enunciado
// Tabuleiro inicial:
//   e c a d c
//   d c d e c
//   b d d c e
//   c d e e b
//   a c c b b

void preencherTabuleiro(char **tabuleiro, int linhas, int colunas) {
    char init[5][5] = {
        {'e', 'c', 'a', 'd', 'c'},
        {'d', 'c', 'd', 'e', 'c'},
        {'b', 'd', 'd', 'c', 'e'},
        {'c', 'd', 'e', 'e', 'b'},
        {'a', 'c', 'c', 'b', 'b'}
    };
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = init[i][j];
        }
    }
}

// Testa se a criaTabuleiro aloca corretamente a memória para um tabuleiro 5x5
void test_criaTabuleiro(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    CU_ASSERT_PTR_NOT_NULL(tab);
    for (int i = 0; i < linhas; i++) {
        CU_ASSERT_PTR_NOT_NULL(tab[i]);
    }
    // Liberta a memória alocada
    for (int i = 0; i < linhas; i++) {
        free(tab[i]);
    }
    free(tab);
}

// Testa se a pintaBranco converte a letra para maiúscula
void test_pintaBranco(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiro(tab, linhas, colunas);
    
    Coordenadas ponto;
    // Testa a coordenada a1 (linha 0, coluna 0): 'e' deve tornar-se 'E'
    ponto.x = 0; // 'a' corresponde à coluna 0
    ponto.y = 0; // 1 corresponde à linha 0
    pintaBranco(tab, linhas, colunas, ponto);
    CU_ASSERT_EQUAL(tab[0][0], 'E');
    
    // Testa outra coordenada: d2 (linha 1, coluna 3)
    // Na linha 1 (segunda linha): { 'd', 'c', 'd', 'e', 'c' }
    // A casa na coluna 3 é 'e', que deve tornar-se 'E'
    ponto.x = 3; 
    ponto.y = 1;
    pintaBranco(tab, linhas, colunas, ponto);
    CU_ASSERT_EQUAL(tab[1][3], 'E');
    
    // Liberta a memória
    for (int i = 0; i < linhas; i++) {
        free(tab[i]);
    }
    free(tab);
}

// Testa se riscaQuadrado substitui a letra por '#'
void test_riscaQuadrado(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiro(tab, linhas, colunas);
    
    Coordenadas ponto;
    // Testa a coordenada c3 (linha 2, coluna 2)
    // Na linha 2: { 'b', 'd', 'd', 'c', 'e' } -> a posição [2][2] contém 'd'
    ponto.x = 2;
    ponto.y = 2;
    riscaQuadrado(tab, linhas, colunas, ponto);
    CU_ASSERT_EQUAL(tab[2][2], '#');
    
    // Testa outra coordenada, por exemplo, b1 (linha 0, coluna 1)
    // Na linha 0: { 'e', 'c', 'a', 'd', 'c' } -> posição [0][1] contém 'c'
    ponto.x = 1;
    ponto.y = 0;
    riscaQuadrado(tab, linhas, colunas, ponto);
    CU_ASSERT_EQUAL(tab[0][1], '#');
    
    // Liberta a memória
    for (int i = 0; i < linhas; i++) {
        free(tab[i]);
    }
    free(tab);
}

// Testa que, quando se fornecem coordenadas inválidas, o tabuleiro não é alterado
void test_coordenadaInvalida(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiro(tab, linhas, colunas);
    
    // Salva o valor original de uma casa, neste caso, a1
    char original = tab[0][0];  // 'e'
    
    Coordenadas ponto;
    // Coordenada inválida: fora do limite (6, 0) [linha 7 não existe]
    ponto.x = 0;
    ponto.y = 6;
    pintaBranco(tab, linhas, colunas, ponto);
    CU_ASSERT_EQUAL(tab[0][0], original);
    
    // Testa com riscaQuadrado com coordenada inválida (0, 6)
    ponto.x = 0;
    ponto.y = 6;
    riscaQuadrado(tab, linhas, colunas, ponto);
    CU_ASSERT_EQUAL(tab[0][0], original);
    
    // Liberta a memória
    for (int i = 0; i < linhas; i++) {
        free(tab[i]);
    }
    free(tab);
}

int main(void) {
    // Inicializa o registro do CUnit
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }
    
    // Adiciona uma suite de testes
    CU_pSuite suite = CU_add_suite("Suite Etapa1", 0, 0);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // Adiciona os testes à suite
    if (CU_add_test(suite, "Teste criaTabuleiro", test_criaTabuleiro) == NULL ||
        CU_add_test(suite, "Teste pintaBranco", test_pintaBranco) == NULL ||
        CU_add_test(suite, "Teste riscaQuadrado", test_riscaQuadrado) == NULL ||
        CU_add_test(suite, "Teste coordenadas inválidas", test_coordenadaInvalida) == NULL)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // Executa os testes com modo verboso
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return CU_get_error();
}
