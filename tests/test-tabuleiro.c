#include <CUnit/Basic.h>
#include <stdlib.h>
#include <ctype.h>
#include "tabuleiro.h"

/* Função auxiliar: preenche o tabuleiro com um padrão fixo (5x5) */
void preencherTabuleiroTeste(char **tabuleiro, int linhas, int colunas) {
    /* Padrão para teste (5x5) */
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

void test_pintaBrancoValido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    Coordenadas c = {0, 0};  // Original 'e'
    int res = pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[0][0], 'E');

    /* Teste: pintar outro ponto válido */
    Coordenadas c2 = {3, 1};  // Na linha 1, col 3: originalmente 'e'
    res = pintaBranco(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[1][3], 'E');

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

void test_pintaBrancoInvalido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    /* Pintar uma célula válida */
    Coordenadas c = {1, 0};  // letra 'c'
    int res = pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[0][1], 'C');

    /* Tentar pintar a mesma célula novamente */
    res = pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(tab[0][1], 'C');

    /* Riscar uma célula e tentar pintá-la */
    Coordenadas c2 = {2, 2}; // originalmente, em linha 2: 'd'
    res = riscaQuadrado(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[2][2], '#');
    res = pintaBranco(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(tab[2][2], '#');

    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

void test_riscaQuadradoValido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    /* Para que riscar seja válido, segundo a implementação, as casas vizinhas ortogonais 
       devem estar pintadas de branco. Vamos pintar essas vizinhas e depois riscar a célula alvo.
       Escolhemos a célula (2,2) e seus vizinhos: acima (2,1), abaixo (2,3), esquerda (1,2) e direita (3,2). */
    Coordenadas up = {2, 1};
    Coordenadas down = {2, 3};
    Coordenadas left = {1, 2};
    Coordenadas right = {3, 2};
    int res;
    res = pintaBranco(tab, linhas, colunas, up);
    CU_ASSERT_EQUAL(res, 1);
    res = pintaBranco(tab, linhas, colunas, down);
    CU_ASSERT_EQUAL(res, 1);
    res = pintaBranco(tab, linhas, colunas, left);
    CU_ASSERT_EQUAL(res, 1);
    res = pintaBranco(tab, linhas, colunas, right);
    CU_ASSERT_EQUAL(res, 1);
    
    Coordenadas target = {2, 2};
    res = riscaQuadrado(tab, linhas, colunas, target);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(tab[2][2], '#');
    
    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

void test_riscaQuadradoInvalido(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    /* Tentar riscar uma célula já pintada (não é permitido) */
    Coordenadas c = {0, 0};
    int res = pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);
    res = riscaQuadrado(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 0);
    CU_ASSERT_EQUAL(tab[0][0], 'E');

    /* Tentar riscar uma célula sem que as vizinhas estejam pintadas */
    Coordenadas c2 = {4, 4};
    res = riscaQuadrado(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 0);
    
    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

void test_converteParaMinuscula(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    /* Pintar uma célula para transformá-la em branca (maiúscula) */
    Coordenadas c = {1, 0};  // originalmente 'c'
    int res = pintaBranco(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_TRUE(isupper(tab[0][1]));

    /* Converter para minúscula */
    res = converteParaMinuscula(tab, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_TRUE(islower(tab[0][1]));

    /* Tentar converter uma célula que não está em maiúscula */
    Coordenadas c2 = {2, 2};
    res = converteParaMinuscula(tab, linhas, colunas, c2);
    CU_ASSERT_EQUAL(res, 0);
    
    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

void test_coordenadaInvalida(void) {
    int linhas = 5, colunas = 5;
    char **tab = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tab, linhas, colunas);

    Coordenadas invalida = {0, 6}; // fora dos limites: 6ª linha em um tabuleiro 5x5
    int res = pintaBranco(tab, linhas, colunas, invalida);
    CU_ASSERT_EQUAL(res, 0);
    
    res = riscaQuadrado(tab, linhas, colunas, invalida);
    CU_ASSERT_EQUAL(res, 0);
    
    for (int i = 0; i < linhas; i++)
        free(tab[i]);
    free(tab);
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Suite_Tabuleiro", 0, 0);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (!CU_add_test(suite, "Teste criaTabuleiro", test_criaTabuleiro) ||
        !CU_add_test(suite, "Teste pintaBranco Válido", test_pintaBrancoValido) ||
        !CU_add_test(suite, "Teste pintaBranco Inválido", test_pintaBrancoInvalido) ||
        !CU_add_test(suite, "Teste riscaQuadrado Válido", test_riscaQuadradoValido) ||
        !CU_add_test(suite, "Teste riscaQuadrado Inválido", test_riscaQuadradoInvalido) ||
        !CU_add_test(suite, "Teste converteParaMinuscula", test_converteParaMinuscula) ||
        !CU_add_test(suite, "Teste coordenada inválida", test_coordenadaInvalida)
       ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
