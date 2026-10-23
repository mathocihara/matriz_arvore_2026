
#ifndef MATRIZ_H
#define MATRIZ_H

typedef struct No {
    int valor;

    struct No *esquerda;
    struct No *direita;
    struct No *cima;
    struct No *baixo;
} No;

typedef struct {
    No *inicio;
    int linhas;
    int colunas;
} Matriz;

#endif
