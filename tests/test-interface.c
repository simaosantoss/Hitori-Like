#include <CUnit/Basic.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "tabuleiro.h"
#include "stack.h"

/* Função auxiliar */
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

/* Mocks para simular interação (testes de interface baseados em funções diretas) */

void testInterfacePintaBrancoComandoValido(void) {
    int linhas = 5, colunas = 5;
    char **tabuleiro = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tabuleiro, linhas, colunas);
    StackMovimentos stack;
    inicializarStack(&stack);

    Coordenadas c = {0, 0};
    char valorAntigo = tabuleiro[c.y][c.x];
    int res = pintaBranco(tabuleiro, linhas, colunas, c);
    CU_ASSERT_EQUAL(res, 1);

    // Criação do movimento e empilhamento
    Movimento mov = { c.x, c.y, valorAntigo, tabuleiro[c.y][c.x] };
    push(&stack, mov);

    // Verifica se a pilha não está vazia
    CU_ASSERT_PTR_NOT_NULL(stack.topo);

    libertaMemoria(tabuleiro, linhas);
    destruirStack(&stack);
}

void testInterfacePintaBrancoComandoInvalido(void) {
    int linhas = 5, colunas = 5;
    char **tabuleiro = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tabuleiro, linhas, colunas);

    StackMovimentos stack;
    inicializarStack(&stack);

    Coordenadas c = {9, 9};
    int res = pintaBranco(tabuleiro, linhas, colunas, c);
    if (res != 0) {
        CU_FAIL("Pintura deveria falhar com coordenada invalida.");
    }

    libertaMemoria(tabuleiro, linhas);
    destruirStack(&stack);
}

void testInterfaceRiscaQuadradoSemVizinhosBrancos(void) {
    int linhas = 5, colunas = 5;
    char **tabuleiro = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tabuleiro, linhas, colunas);
    StackMovimentos stack;
    inicializarStack(&stack);

    Coordenadas c = {2, 2};
    int res = riscaQuadrado(tabuleiro, linhas, colunas, c);
    if (res != 0) {
        CU_FAIL("Riscar deveria falhar sem vizinhos brancos.");
    }

    libertaMemoria(tabuleiro, linhas);
    destruirStack(&stack);
}

void testInterfaceDesfazerMovimento(void) {
    int linhas = 5, colunas = 5;
    char **tabuleiro = criaTabuleiro(linhas, colunas);
    preencherTabuleiroTeste(tabuleiro, linhas, colunas);
    StackMovimentos stack;
    inicializarStack(&stack);

    Coordenadas c = {0, 0};
    char valorAntigo = tabuleiro[c.y][c.x];
    pintaBranco(tabuleiro, linhas, colunas, c);
    Movimento mov = {c.x, c.y, valorAntigo, tabuleiro[c.y][c.x]};
    push(&stack, mov);

    int res = pop(&stack, &mov);
    CU_ASSERT_EQUAL(res, 1);
    tabuleiro[mov.y][mov.x] = mov.valorAntigo;
    CU_ASSERT_EQUAL(tabuleiro[mov.y][mov.x], valorAntigo);

    libertaMemoria(tabuleiro, linhas);
    destruirStack(&stack);
}

void testInterfaceDesfazerStackVazia(void) {
    StackMovimentos stack;
    inicializarStack(&stack);
    Movimento mov;
    int res = pop(&stack, &mov);
    if (res != 0) {
        CU_FAIL("Pop deveria falhar com stack vazia.");
    }
    destruirStack(&stack);
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Suite_Interface", 0, 0);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (!CU_add_test(suite, "Testa comando de pintar branco válido", testInterfacePintaBrancoComandoValido) ||
        !CU_add_test(suite, "Testa comando de pintar branco inválido", testInterfacePintaBrancoComandoInvalido) ||
        !CU_add_test(suite, "Testa riscar quadrado sem vizinhos brancos", testInterfaceRiscaQuadradoSemVizinhosBrancos) ||
        !CU_add_test(suite, "Testa desfazer movimento válido", testInterfaceDesfazerMovimento) ||
        !CU_add_test(suite, "Testa desfazer com stack vazia", testInterfaceDesfazerStackVazia)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
