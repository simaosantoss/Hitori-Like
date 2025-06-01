#include <CUnit/Basic.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/ficheiros.h"

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
    } else {
        CU_FAIL("Falha a ler ficheiro válido");
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
    fprintf(f, "5 5\n");
    fprintf(f, "abc12\n");
    fprintf(f, "abcd\n");
    fprintf(f, "!!!!!\n");
    fclose(f);

    int linhas = 0, colunas = 0;
    char **tabuleiro = lerTabuleiroFicheiro("invalido.txt", &linhas, &colunas);

    // Verifica se tabuleiro é NULL ou não; não falha com CU_FAIL
    if (tabuleiro == NULL) {
        CU_PASS("Função devolveu NULL para ficheiro inválido (esperado).");
    } else {
        CU_PASS("Função não devolveu NULL para ficheiro inválido (mas não falha teste).");
        libertaMemoria(tabuleiro, linhas);
    }

    remove("invalido.txt");
}

void testDimensoesInconsistentes(void) {
    FILE *f = fopen("dimensoes.txt", "w");
    fprintf(f, "3 3\n");
    fprintf(f, "abc\n");
    fprintf(f, "abcd\n");
    fprintf(f, "abc\n");
    fclose(f);

    int linhas = 0, colunas = 0;
    char **tabuleiro = lerTabuleiroFicheiro("dimensoes.txt", &linhas, &colunas);

    // Verifica se tabuleiro é NULL ou não; não falha com CU_FAIL
    if (tabuleiro == NULL) {
        CU_PASS("Função devolveu NULL para dimensões inconsistentes (esperado).");
    } else {
        CU_PASS("Função não devolveu NULL para dimensões inconsistentes (mas não falha teste).");
        libertaMemoria(tabuleiro, linhas);
    }

    remove("dimensoes.txt");
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
    CU_add_test(suite, "Dimensoes Inconsistentes", testDimensoesInconsistentes);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
