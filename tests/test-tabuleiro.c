#include <CUnit/Basic.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/tabuleiro.h"

// Cria um tabuleiro de teste
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

// Teste para verificar a criação do tabuleiro
void testCriaTabuleiro(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c);
    CU_ASSERT_PTR_NOT_NULL(tab);
    for (int i = 0; i < l; ++i) CU_ASSERT_PTR_NOT_NULL(tab[i]);
    for (int i = 0; i < l; ++i) free(tab[i]);
    free(tab);
}

// Teste para pintura válida de uma célula do tabuleiro
void testPintaBrancoValido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); 
    preencherTabuleiroTeste(tab, l, c);

    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, (Coordenadas){0,0}), 1);
    CU_ASSERT_EQUAL(tab[0][0], 'E');
    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, (Coordenadas){3,1}), 1);
    CU_ASSERT_EQUAL(tab[1][3], 'E');
    for (int i = 0; i < l; ++i) free(tab[i]);
    free(tab);
}

// Teste para tentar pintar uma célula já branca
void testPintaBrancoInvalido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); 
    preencherTabuleiroTeste(tab, l, c);

    Coordenadas c1 = {1,0};
    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, c1), 1);
    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, c1), 0);           
    for (int i = 0; i < l; ++i) free(tab[i]);
    free(tab);
}

// Teste para riscar um quadrado corretamente
void testRiscaQuadradoValido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); 
    preencherTabuleiroTeste(tab, l, c);

    Coordenadas v[4] = {{2,1},{2,3},{1,2},{3,2}};
    for (int i = 0; i < 4; ++i) pintaBranco(tab, l, c, v[i]);

    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, (Coordenadas){2,2}), 1);
    CU_ASSERT_EQUAL(tab[2][2], '#');
    for (int i = 0; i < l; ++i) free(tab[i]); 
    free(tab);
}

// Teste para tentativa de riscar um quadrado sem vizinhos brancos
void testRiscaQuadradoInvalido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); 
    preencherTabuleiroTeste(tab, l, c);

    pintaBranco(tab, l, c, (Coordenadas){0,0});
    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, (Coordenadas){0,0}), 0); 
    for (int i = 0; i < l; ++i) free(tab[i]); 
    free(tab);
}

// Teste para conversão de célula para minúscula
void testConverteParaMinuscula(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); 
    preencherTabuleiroTeste(tab, l, c);

    Coordenadas p = {1,0};
    pintaBranco(tab, l, c, p);
    CU_ASSERT_EQUAL(converteParaMinuscula(tab, p), 1);
    CU_ASSERT_TRUE(islower(tab[0][1]));
    for (int i = 0; i < l; ++i) free(tab[i]); 
    free(tab);
}

// Teste para coordenadas inválidas
void testCoordenadaInvalida(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c); 
    preencherTabuleiroTeste(tab, l, c);

    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, (Coordenadas){-1,0}), 0);
    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, (Coordenadas){5,5}), 0);
    for (int i = 0; i < l; ++i) free(tab[i]); 
    free(tab);
}

void testComandoA(void) {
    int l = 3, c = 3;
    char **tab = criaTabuleiro(l, c);
    // Tabuleiro com minúsculas para garantir alterações
    char init[3][3] = {
        {'A', 'a', 'C'},
        {'d', 'E', 'f'},
        {'G', 'h', 'I'}
    };
    for (int y=0; y<l; y++)
        for (int x=0; x<c; x++)
            tab[y][x] = init[y][x];

    int mudou = aplica_comando_a(tab, l, c);
    CU_ASSERT_TRUE(mudou);

    for (int i=0; i<l; i++) free(tab[i]);
    free(tab);
}

void testComandoAIterativo(void) {
    int l = 3, c = 3;
    char **tab = criaTabuleiro(l, c);

    // Tabuleiro com muitas minúsculas para forçar alterações
    char init[3][3] = {
        {'a', 'b', 'c'},
        {'d', 'e', 'f'},
        {'g', 'h', 'i'}
    };
    for (int y = 0; y < l; y++)
        for (int x = 0; x < c; x++)
            tab[y][x] = init[y][x];

    int iteracoes = aplica_comando_A(tab, l, c);
    CU_ASSERT_TRUE(iteracoes >= 0);

    for (int i = 0; i < l; i++) free(tab[i]);
    free(tab);
}

// Teste para resolver um jogo basico
void testResolverJogoSimples(void) {
    int l = 3, c = 3;
    char **tab = criaTabuleiro(l, c);

    char init[3][3] = {
        {'A', 'b', 'C'},
        {'d', 'E', 'f'},
        {'G', 'h', 'I'}
    };
    for (int y=0; y<l; y++)
        for (int x=0; x<c; x++)
            tab[y][x] = init[y][x];

    int res = resolverJogo(tab, l, c);
    CU_ASSERT_TRUE(res == 1 || res == 0);

    for (int i=0; i<l; i++) free(tab[i]);
    free(tab);
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) 
        return CU_get_error();

    CU_pSuite s = CU_add_suite("Suite_Tabuleiro", 0, 0);
    if (!s) { 
        CU_cleanup_registry(); 
        return CU_get_error(); 
    }

    CU_add_test(s, "Criação do Tabuleiro", testCriaTabuleiro);
    CU_add_test(s, "Pintura Válida", testPintaBrancoValido);
    CU_add_test(s, "Pintura Inválida", testPintaBrancoInvalido);
    CU_add_test(s, "Riscar Válido", testRiscaQuadradoValido);
    CU_add_test(s, "Riscar Inválido", testRiscaQuadradoInvalido);
    CU_add_test(s, "Conversão para Minúscula", testConverteParaMinuscula);
    CU_add_test(s, "Coordenadas Inválidas", testCoordenadaInvalida);
    CU_add_test(s, "Comando a", testComandoA);
    CU_add_test(s, "Comando A iterativo", testComandoAIterativo);
    CU_add_test(s, "Resolver jogo simples", testResolverJogoSimples);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
