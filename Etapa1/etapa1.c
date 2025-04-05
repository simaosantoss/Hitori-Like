#include <stdio.h>
#include <ctype.h>  // Necessário para a função toupper()

// isto está cheio de comentários mas não é com GPT, é para vocês perceberem as mudanças
// caso haja algo que não gostem mudem ou se acharem que estava melhor antes é só ir ao histórico de commits e revertem
// ATENÇÃO: fiz este commit apenas para ficar no historico, o que tenho a seguir já está feito e testado no meu pc e vou copiar e colar para o github agora, para vocês poderem ver tanto esta versão no historico como a mais recente que segue mais o enunciado
 
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

// está no enunciado que "<coordenada> no formato <letra minúscula><número> onde a letra corresponde a uma coluna e o número a uma linha"
// mas nós tinhamos coordenada como int int e não como char int
void pintaBranco(int linhas, int colunas) {
    Coordenadas ponto;
    imprimeTabuleiro(tabuleiro, 5, 5);
    
    // Solicita as coordenadas para pintar de branco
    printf("Que casa pretende tornar maiúscula? \nInsira as coordenadas (ex: a1): ");
    char letra;
    int numero;

    // Ler a coordenada - mudado para aceitar letra e número (ex: a1, b3, etc)
    if (scanf(" %c%d", &letra, &numero) != 2) {
        printf("Erro! Input incorreto.\n");
        return;
    }
    
    // Converter a letra para um índice de coluna
    ponto.x = letra - 'a';  // 'a' corresponde a 0, 'b' a 1, etc. (corrigido)
    ponto.y = numero - 1;   // Ajuste de 1-based para 0-based (corrigido)

    // Verificar se as coordenadas estão dentro do tabuleiro
    if (ponto.x >= 0 && ponto.x < colunas && ponto.y >= 0 && ponto.y < linhas) {
        tabuleiro[ponto.y][ponto.x] = toupper(tabuleiro[ponto.y][ponto.x]); // Usar toupper (corrigido)
    } else {
        printf("Coordenada inválida!\n");
    }
}

// mesma coisa da função anterior, mas para riscar

void riscaQuadrado(int linhas, int colunas) {
    Coordenadas ponto;
    imprimeTabuleiro(tabuleiro, 5, 5);
    
    // Solicitar coordenadas para riscar
    printf("Que casa pretende riscar? \nInsira as coordenadas (ex: a1): ");
    char letra;
    int numero;

    // Ler a coordenada - mudado para aceitar letra e número (ex: a1, b3, etc)
    if (scanf(" %c%d", &letra, &numero) != 2) {
        printf("Erro! Input incorreto.\n");
        return;
    }
    
    // Converter a letra para um índice de coluna
    ponto.x = letra - 'a';
    ponto.y = numero - 1;

    // Verificar se as coordenadas estão dentro do tabuleiro
    if (ponto.x >= 0 && ponto.x < colunas && ponto.y >= 0 && ponto.y < linhas) {
        tabuleiro[ponto.y][ponto.x] = '#';
    } else {
        printf("Coordenada inválida!\n");
    }
    
    imprimeTabuleiro(tabuleiro, 5, 5);
}

int main () {
    pintaBranco(5, 5);
    riscaQuadrado(5, 5);
    return 0;
}