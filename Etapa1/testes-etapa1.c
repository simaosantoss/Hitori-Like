#include <stdio.h>
#include <ctype.h>
#include "etapa1.h"
#include <CUnit/Basic.h>  // Inclusão do CUnit para os testes -> isto está me a dar erro mas eu estive a ver a documentação CUnit na net e acho que isto está bem, é parecido com HUnit do haskell
// só não sei dar labels a testes

// !! 
// A makefile ainda não tem isto porque não pode pelo menos no meu pc em que o cunit ainda não está bem configurado

// Teste para verificar a função 'pintaBranco'
void test_pintaBranco() {
    Coordenadas ponto;

    // Coordenada a1 (casa da letra 'e')
    ponto.x = 0; ponto.y = 0;  // Coordenada a1
    pintaBranco(5, 5, ponto);
    CU_ASSERT_EQUAL(tabuleiro[ponto.y][ponto.x], 'E');  // Deve ser 'E' após ser pintada de branco
}

// Teste para verificar a função 'riscaQuadrado'
void test_riscaQuadrado() {
    Coordenadas ponto;

    // Coordenada c3 (casa da letra 'd')
    ponto.x = 2; ponto.y = 2;  // Coordenada c3
    riscaQuadrado(5, 5, ponto);
    CU_ASSERT_EQUAL(tabuleiro[ponto.y][ponto.x], '#');  // Deve ser '#' após ser riscada
}

// Teste para coordenadas inválidas
void test_coordenadaInvalida() {
    Coordenadas ponto;

    // Coordenada fora dos limites do tabuleiro (5,5)
    ponto.x = 5; ponto.y = 5;
    pintaBranco(5, 5, ponto);  // Não deve alterar nada
    CU_ASSERT_EQUAL(tabuleiro[4][4], 'b');  // A última casa não deve ser alterada
}


// ok, esta main foi chat porque eu nao sabia fazer main aqui, e não a percebi. Só meti aqui para ver se dava bem e para ver se vocês queriam ver. Tudo o que está acima
// foi TOTALMENTE sem chat, porque é como haskell basicamente. Esta main ou se altera ou se percebe, eu não a consigo perceber agora (é uma da manhã)

int main() {
    // Inicializa os testes
    CU_initialize_registry();
    
    // Adiciona a suite de testes
    CU_pSuite suite = CU_add_suite("Testes Tabuleiro", 0, 0);
    
    // Adiciona os testes à suite
    CU_add_test(suite, "Testar pintaBranco", test_pintaBranco);
    CU_add_test(suite, "Testar riscaQuadrado", test_riscaQuadrado);
    CU_add_test(suite, "Testar coordenadas inválidas", test_coordenadaInvalida);
    
    // Executa os testes
    CU_basic_run_tests();
    
    // Finaliza os testes
    CU_cleanup_registry();
    
    return 0;
}
