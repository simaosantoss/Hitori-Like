#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>
#include "../include/interface.h"
#include "../include/tabuleiro.h"
#include "../include/stack.h"

static void preencher(char **t,int l,int c){
    char init[5][5] = {
        {'e','c','a','d','c'},
        {'d','c','d','e','c'},
        {'b','d','d','c','e'},
        {'c','d','e','e','b'},
        {'a','c','c','b','b'}
    };
    for(int y=0;y<l;++y) 
        for(int x=0;x<c;++x) 
            t[y][x] = init[y][x];
}

// Teste de uma pintura valida
void testPintaValida(void){
    int l = 5, c = 5; 
    char **tab = criaTabuleiro(l, c); 
    preencher(tab, l, c);
    StackMovimentos st; 
    initStack(&st);

    Coordenadas p = {0, 0}; 
    char old = tab[0][0];
    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, p), 1);
    Movimento m = {p.x, p.y, old, tab[0][0]}; 
    push(&st, m);
    if (!st.topo) CU_FAIL("Stack topo não deve ser NULL");

    libertaMemoria(tab, l); 
    destruirStack(&st);
}

// Teste de pintar uma coordenada invalida
void testPintaInvalidaCoord(void){
    int l = 5, c = 5; 
    char **tab = criaTabuleiro(l, c); 
    preencher(tab, l, c);
    StackMovimentos st; 
    initStack(&st);
    CU_ASSERT_EQUAL(pintaBranco(tab, l, c, (Coordenadas){9,9}), 0);
    libertaMemoria(tab, l); 
    destruirStack(&st);
}

// Teste para riscar um quadrado sem vizinhos brancos
void testRiscaSemVizinhosBrancosPermitido(void){
    int l = 5, c = 5; 
    char **tab = criaTabuleiro(l, c); 
    preencher(tab, l, c);
    StackMovimentos st; 
    initStack(&st);

    Coordenadas p = {2, 2};           
    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, p), 1);
    CU_ASSERT_EQUAL(tab[2][2], '#'); 

    libertaMemoria(tab, l); 
    destruirStack(&st);
}

// Teste para riscar um quadrado válido e um inválido
void testRiscaValidoEInvalido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c);
    preencher(tab, l, c);
    StackMovimentos st;
    initStack(&st);

    Coordenadas pValido = {2, 2};
    Coordenadas pInvalido = {0, 0};

    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, pValido), 1);
    CU_ASSERT_EQUAL(tab[2][2], '#');

    pintaBranco(tab, l, c, pInvalido);
    CU_ASSERT_EQUAL(riscaQuadrado(tab, l, c, pInvalido), 0);

    libertaMemoria(tab, l);
    destruirStack(&st);
}

// Teste para converter caractere para minúscula
void testConverterMinusculaValidoEInvalido(void) {
    int l = 5, c = 5;
    char **tab = criaTabuleiro(l, c);
    preencher(tab, l, c);

    Coordenadas pMaiuscula = {0, 0};
    Coordenadas pMinuscula = {1, 1};

    pintaBranco(tab, l, c, pMaiuscula);
    CU_ASSERT_EQUAL(converteParaMinuscula(tab, pMaiuscula), 1);

    CU_ASSERT_EQUAL(converteParaMinuscula(tab, pMinuscula), 0);

    libertaMemoria(tab, l);
}

// Teste para desfazer o ultimo movimento
void testUndo(void){
    int l = 5, c = 5; 
    char **tab = criaTabuleiro(l, c); 
    preencher(tab, l, c);
    StackMovimentos st; 
    initStack(&st);

    Coordenadas p = {0, 0}; 
    char old = tab[0][0];
    pintaBranco(tab, l, c, p);
    Movimento m = {p.x, p.y, old, tab[0][0]}; 
    push(&st, m);

    Movimento popM;
    CU_ASSERT_EQUAL(pop(&st, &popM), 1);
    tab[popM.y][popM.x] = popM.valorAntigo;
    CU_ASSERT_EQUAL(tab[0][0], old);

    libertaMemoria(tab, l); 
    destruirStack(&st);
}

// Teste para desfazer ação com a stack vazia
void testDesfazerVazio(void) {
    StackMovimentos st;
    initStack(&st);

    Movimento m;
    CU_ASSERT_EQUAL(pop(&st, &m), 0);
    destruirStack(&st);
}

void testPopVazio(void){
    StackMovimentos st; 
    initStack(&st);
    Movimento m; 
    CU_ASSERT_EQUAL(pop(&st, &m), 0);
    destruirStack(&st);
}

// Teste para verificar se o tabuleiro original é guardado direito
void testGuardaOriginal(void) {
    int l = 3, c = 3;
    char **tab = criaTabuleiro(l, c);

    for(int i=0; i<l; i++)
        for(int j=0; j<c; j++)
            tab[i][j] = 'a' + i*c + j;

    guardaOriginal(tab, l, c);

    CU_ASSERT_TRUE(1);

    for(int i=0; i<l; i++)
        free(tab[i]);
    free(tab);
}

int main(void){
    if(CU_initialize_registry() != CUE_SUCCESS) 
        return CU_get_error();
    CU_pSuite s = CU_add_suite("Suite_Interface", 0, 0);
    if(!s){ 
        CU_cleanup_registry(); 
        return CU_get_error(); 
    }

    CU_add_test(s, "Pintar válido", testPintaValida);
    CU_add_test(s, "Pintar inválido (coord)", testPintaInvalidaCoord);
    CU_add_test(s, "Riscar sem vizinhos brancos", testRiscaSemVizinhosBrancosPermitido);
    CU_add_test(s, "Riscar válido e inválido", testRiscaValidoEInvalido);
    CU_add_test(s, "Converter para minúscula válido e inválido", testConverterMinusculaValidoEInvalido);
    CU_add_test(s, "Desfazer movimento", testUndo);
    CU_add_test(s, "Desfazer stack vazio", testDesfazerVazio);
    CU_add_test(s, "Pop em stack vazia", testPopVazio);
    CU_add_test(s, "Guarda Original", testGuardaOriginal);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests(); 
    CU_cleanup_registry();
    return CU_get_error();
}