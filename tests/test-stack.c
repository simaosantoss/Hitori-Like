#include <CUnit/Basic.h>
#include "../include/stack.h"

// Teste para empilhar e desempilhar elementos direito
void test_stack_push_pop(void) {
    StackMovimentos stack;
    initStack(&stack);

    // Inicializa dois movimentos
    Movimento mov1 = { .x = 1, .y = 2, .valorAntigo = 'a', .valorNovo = 'A' };
    Movimento mov2 = { .x = 3, .y = 4, .valorAntigo = 'b', .valorNovo = 'B' };

    // Empilha os movimentos
    push(&stack, mov1);
    push(&stack, mov2);

    // Testa desempilhar o último elemento metido
    Movimento popMov;
    int res = pop(&stack, &popMov);
    CU_ASSERT_EQUAL(res, 1);

    CU_ASSERT_EQUAL(popMov.x, mov2.x);
    CU_ASSERT_EQUAL(popMov.y, mov2.y);
    CU_ASSERT_EQUAL(popMov.valorAntigo, mov2.valorAntigo);
    CU_ASSERT_EQUAL(popMov.valorNovo, mov2.valorNovo);

    // Testa desempilhar o próximo elemento
    res = pop(&stack, &popMov);
    CU_ASSERT_EQUAL(res, 1);
    CU_ASSERT_EQUAL(popMov.x, mov1.x);
    CU_ASSERT_EQUAL(popMov.y, mov1.y);
    CU_ASSERT_EQUAL(popMov.valorAntigo, mov1.valorAntigo);
    CU_ASSERT_EQUAL(popMov.valorNovo, mov1.valorNovo);

    // Testa desempilhar a stack vazia
    res = pop(&stack, &popMov);
    CU_ASSERT_EQUAL(res, 0);
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Suite_Stack", 0, 0);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (!CU_add_test(suite, "Teste push/pop", test_stack_push_pop)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}