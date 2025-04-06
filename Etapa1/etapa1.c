#include <stdio.h>
#include <ctype.h>  // Necessário para a função toupper()
#include "etapa1.h"

// Estes comentarios depois sao apagados manos, isto é só para explicar o que fiz e para não estarem aqui às cegas
// esta versão segue exatamente o que é pedido no enunciado, mas se não perceberem o porque de algo vejam o enunciado mais concretamente a parte "Comandos a implementar"
// foi SEM chat, usei para perceber como ia implementar uma funcionalidade mas de resto foi sem chat
// é muito facil de perceber (se não fosse eu não sabia fazer) mas se tiverem duvidas é só perguntar
// caso haja algo que não gostem mudem ou se acharem que estava melhor antes é só ir ao histórico de commits e revertem
 
typedef struct coordenadas {
    int x;
    int y;
} Coordenadas;

char tabuleiro[5][5] = {
    {'e', 'c', 'a', 'd', 'c'},
    {'d', 'c', 'd', 'e', 'c'},
    {'b', 'd', 'd', 'c', 'e'},
    {'c', 'd', 'e', 'e', 'b'},
    {'a', 'c', 'c', 'b', 'b'}
};

void imprimeTabuleiro(char a[5][5], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c  ", a[i][j]);
        }
        printf("\n");
    }
}

void pintaBranco(int linhas, int colunas, Coordenadas ponto) {
    if (ponto.x >= 0 && ponto.x < colunas && ponto.y >= 0 && ponto.y < linhas) {
        tabuleiro[ponto.y][ponto.x] = toupper(tabuleiro[ponto.y][ponto.x]);
    } else {
        printf("Coordenada inválida!\n");
    }
}

void riscaQuadrado(int linhas, int colunas, Coordenadas ponto) {
    if (ponto.x >= 0 && ponto.x < colunas && ponto.y >= 0 && ponto.y < linhas) {
        tabuleiro[ponto.y][ponto.x] = '#';
    } else {
        printf("Coordenada inválida!\n");
    }
}

int main() {
    char comando[10]; // Comando máximo esperado, por exemplo, "b a1". Meti 10 para não dar erro
    char letra;
    int numero;
    
    // Loop REPL
    while (1) {
        imprimeTabuleiro(tabuleiro, 5, 5);
        printf("Digite um comando (b <coordenada>, r <coordenada>, s para sair): ");
        scanf(" %s", comando);

        if (comando[0] == 's') {
            printf("A sair do programa...\n");
            break;  // Encerra o programa se for s
        }
        
        if (comando[0] == 'b' || comando[0] == 'r') {
            // Lê a coordenada após o comando 'b' ou 'r'
            if (scanf(" %c%d", &letra, &numero) != 2) {
                printf("Erro! Input incorreto.\n");
                continue;  // Continua o loop para tentar novamente
            }

            // Converte a letra da coluna (a -> 0, b -> 1, c -> 2, d -> 3, e -> 4 , ...)
            Coordenadas ponto;
            ponto.x = letra - 'a';  // 'a' corresponde a 0, 'b' a 1, etc, como está acima
            ponto.y = numero - 1;   // Ajuste de 1-based para 0-based

            // Executa a ação correspondente
            if (comando[0] == 'b') {
                pintaBranco(5, 5, ponto);
            } else if (comando[0] == 'r') {
                riscaQuadrado(5, 5, ponto);
            }
        } else {
            printf("Comando inválido!\n");
        }
    }

    return 0;
}