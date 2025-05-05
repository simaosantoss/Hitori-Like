#include <CUnit/Basic.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ficheiros.h"

void testGravarELerValido(void) {
    int linhas = 5, colunas = 5;
    char **tabuleiro = malloc(linhas * sizeof(char *));
    char pattern[5][6] = {
        "ecadc",
        "dcdec",
        "bddce",
        "cdeeb",
        "accbb"
    };

    for (int i = 0; i < linhas; i++) {
        tabuleiro[i] = malloc(colunas * sizeof(char));
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j] = pattern[i][j];
        }
    }

    CU_ASSERT_EQUAL(gravarTabuleiroFicheiro("testeOk.txt", tabuleiro, linhas, colunas), 1);

    int lidas = 0, cols = 0;
    char **lido = lerTabuleiroFicheiro("testeOk.txt", &lidas, &cols);
    CU_ASSERT_PTR_NOT_NULL(lido);
    CU_ASSERT_EQUAL(lidas, linhas);
    CU_ASSERT_EQUAL(cols, colunas);

    if (lido) {
        for (int i = 0; i < linhas; i++) {
            for (int j = 0; j < colunas; j++) {
                CU_ASSERT_EQUAL(lido[i][j], pattern[i][j]);
            }
            free(lido[i]);
        }
        free(lido);
    }

    for (int i = 0; i < linhas; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
    remove("testeOk.txt");
}

void testArquivoInexistente(void) {
    int linhas = 0, colunas = 0;
    char **tabuleiro = lerTabuleiroFicheiro("naoExiste.txt", &linhas, &colunas);
    CU_ASSERT_PTR_NULL(tabuleiro);
}

void testFicheiroVazio(void) {
    FILE *f = fopen("vazio.txt", "w");
    fclose(f);

    int linhas = 0, colunas = 0;
    char **tabuleiro = lerTabuleiroFicheiro("vazio.txt", &linhas, &colunas);
    CU_ASSERT_PTR_NULL(tabuleiro);
    remove("vazio.txt");
}

void testFormatoInvalido(void) {
    FILE *f = fopen("invalido.txt", "w");
    fprintf(f, "isto nao tem letras do tabuleiro\noutra linha mal\n");
    fclose(f);

    int linhas = 0, colunas = 0;
    char **tabuleiro = lerTabuleiroFicheiro("invalido.txt", &linhas, &colunas);
    if (tabuleiro == NULL) {
        CU_FAIL("Erro esperado: Ficheiro inválido não deve ser lido corretamente");
    } else {
        libertaMemoria(tabuleiro, linhas);
    }

    remove("invalido.txt");
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    CU_pSuite suite = CU_add_suite("TestesFicheiros", NULL, NULL);
    if (!suite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, "Gravar e Ler Tabuleiro Valido", testGravarELerValido);
    CU_add_test(suite, "Ficheiro Inexistente", testArquivoInexistente);
    CU_add_test(suite, "Ficheiro Vazio", testFicheiroVazio);
    CU_add_test(suite, "Formato Invalido", testFormatoInvalido);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
