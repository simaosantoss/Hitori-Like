#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "ficheiros.h"

void test_gravarElerFicheiro(void) {
    /* Cria um tabuleiro 5x5 de teste */
    int linhas = 5, colunas = 5;
    char **tabuleiro = malloc(linhas * sizeof(char *));
    for (int i = 0; i < linhas; i++) {
        tabuleiro[i] = malloc(colunas * sizeof(char));
    }

    /* Padrão esperado */
    char pattern[5][6] = {
        "ecadc",
        "dcdec",
        "bddce",
        "cdeeb",
        "accbb"
    };

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = pattern[i][j];
        }
    }

    /* Grava para um ficheiro temporário */
    int res = gravarEmFicheiro("temp_test.txt", tabuleiro, linhas, colunas);
    CU_ASSERT_EQUAL(res, 1);

    /* Agora, lê de volta o ficheiro */
    char **lido = lerTabuleiroPorDimensoes("temp_test.txt", &linhas, &colunas);
    CU_ASSERT_PTR_NOT_NULL(lido);
    if (lido) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                CU_ASSERT_EQUAL(lido[i][j], pattern[i][j]);
            }
        }
        for (int i = 0; i < 5; i++) {
            free(lido[i]);
        }
        free(lido);
    }

    /* Limpa o ficheiro temporário */
    remove("temp_test.txt");

    for (int i = 0; i < 5; i++)
        free(tabuleiro[i]);
    free(tabuleiro);
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("Suite_Ficheiros", 0, 0);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (!CU_add_test(suite, "Teste gravar e ler ficheiro", test_gravarElerFicheiro)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
