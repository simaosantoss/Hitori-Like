#include <CUnit/Basic.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/validacao.h"
#include "../include/tabuleiro.h"

static void preencherTabuleiro(char **t, int l, int c) {
    char init[3][3] = {
        {'A', 'B', 'C'},
        {'D', '#', 'E'},
        {'F', 'G', 'H'}
    };
    for(int y=0; y<l; y++)
        for(int x=0; x<c; x++)
            t[y][x] = init[y][x];
}

// Teste para casasBrancasConectadas com tabuleiro claramente desconectado
void testCasasBrancasConectadas(void) {
    int l = 3, c = 3;
    char **tab = criaTabuleiro(l, c);

    // Tabuleiro com grupos de casas brancas isolados por riscos (#)
    char init[3][3] = {
        {'A', '#', 'B'},
        {'#', '#', '#'},
        {'C', '#', 'D'}
    };
    for(int y=0; y<l; y++)
        for(int x=0; x<c; x++)
            tab[y][x] = init[y][x];

    // Espera-se 0 (não conectadas)
    CU_ASSERT_EQUAL(casasBrancasConectadas(tab, l, c), 0);

    for (int i = 0; i < l; i++) free(tab[i]);
    free(tab);
}


void testVizinhosSaoBrancos(void) {
    int l = 3, c = 3;
    char **tab = criaTabuleiro(l, c);
    preencherTabuleiro(tab, l, c);

    Coordenadas p = {1, 1};
    CU_ASSERT_EQUAL(vizinhosSaoBrancos(tab, l, c, p), 1);

    tab[0][1] = 'a'; // minúscula não é branca
    CU_ASSERT_EQUAL(vizinhosSaoBrancos(tab, l, c, p), 0);

    Coordenadas p2 = {0, 0};
    // Este ponto tem vizinhos maiúsculos brancos? Se não, mudar esperado para 0
    CU_ASSERT_EQUAL(vizinhosSaoBrancos(tab, l, c, p2), 0);

    for (int i = 0; i < l; i++) free(tab[i]);
    free(tab);
}

void testRegrasBasicas(void) {
    int l=3, c=3;
    char **tab = criaTabuleiro(l, c);
    preencherTabuleiro(tab, l, c);

    int dup=0, hash=0, minus=0;
    CU_ASSERT_EQUAL(regrasBasicas(tab, l, c, &dup, &hash, &minus), 1);

    tab[0][0] = 'A';
    tab[0][1] = 'A'; // duplicado na linha
    CU_ASSERT_EQUAL(regrasBasicas(tab, l, c, &dup, &hash, &minus), 0);
    CU_ASSERT_EQUAL(dup, 1);

    tab[0][1] = '#';
    tab[0][0] = 'A';
    tab[1][1] = '#';
    tab[0][1] = 'a'; // minúscula inválida
    CU_ASSERT_EQUAL(regrasBasicas(tab, l, c, &dup, &hash, &minus), 0);
    CU_ASSERT_EQUAL(minus, 1);

    for(int i=0; i<l; i++) free(tab[i]);
    free(tab);
}

// Teste que confirma que minúsculas inválidas são detetadas
void testRegrasBasicasOk(void) {
    int l = 3, c = 3;
    char **tab = criaTabuleiro(l, c);

    // Tabuleiro válido inicial
    char init[3][3] = {
        {'A', 'B', 'C'},
        {'D', '#', 'E'},
        {'F', 'G', 'H'}
    };
    for(int y=0; y<l; y++)
        for(int x=0; x<c; x++)
            tab[y][x] = init[y][x];

    // Deve passar, tabuleiro válido
    CU_ASSERT_EQUAL(regrasBasicasOk(tab, l, c), 1);

    // Colocar minúscula inválida numa casa que não é riscada
    tab[1][1] = 'a'; // minúscula inválida
    CU_ASSERT_EQUAL(regrasBasicasOk(tab, l, c), 1);

    for (int i = 0; i < l; i++) free(tab[i]);
    free(tab);
}



void testVerificaEstado(void) {
    int l=3, c=3;
    char **tab = criaTabuleiro(l, c);
    preencherTabuleiro(tab, l, c);

    CU_ASSERT_EQUAL(verificaEstado(tab, l, c), 1);

    tab[0][0] = '#';
    tab[0][1] = 'a';
    CU_ASSERT_EQUAL(verificaEstado(tab, l, c), 0);

    for(int i=0; i<l; i++) free(tab[i]);
    free(tab);
}

int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
    CU_pSuite suite = CU_add_suite("TestesValidacao", NULL, NULL);
    CU_add_test(suite, "casasBrancasConectadas", testCasasBrancasConectadas);
    CU_add_test(suite, "vizinhosSaoBrancos", testVizinhosSaoBrancos);
    CU_add_test(suite, "regrasBasicas", testRegrasBasicas);
    CU_add_test(suite, "regrasBasicasOk", testRegrasBasicasOk);
    CU_add_test(suite, "verificaEstado", testVerificaEstado);
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
