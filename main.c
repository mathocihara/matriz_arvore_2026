#include "matriz.h"
#include <stdio.h>
#include <stdlib.h>
 
int main(void) {
    int linhas, colunas;
 
    printf("Digite a quantidade de linhas: ");
    scanf("%d", &linhas);
 
    printf("Digite a quantidade de colunas: ");
    scanf("%d", &colunas);
 
    Matriz *matriz = criarMatriz(linhas, colunas);
 
    if (matriz == NULL) {
        printf("Erro ao criar a matriz!\n");
        return 1;
    }
 
    MENU(matriz);
 
    desalocarMatriz(matriz);
 
    return 0;
}
 