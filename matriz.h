#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


Matriz* criarMatriz(int linhas, int colunas);
void desalocarMatriz(Matriz *m);

int validarPosicao(Matriz *m, int linha, int coluna);
No* acessarPosicao(Matriz *m, int linha, int coluna);
int inserirValor(Matriz *m, int linha, int coluna, int valor);
No* buscarValor(Matriz *m, int valor);
void imprimirVizinhos(Matriz *m, int linha, int coluna);
void adicionarLinha(Matriz *m);
void adicionarColuna(Matriz *m);
int removerLinha(Matriz *m, int linha);
int removerColuna(Matriz *m, int coluna);
void imprimirMatriz(Matriz *m);
int matrizVazia(Matriz *m);
No* localizarElemento(Matriz *m, int linha, int coluna);
No* criarNo(int valor);

Matriz* lerECriarMatriz(void);
Matriz* menu(Matriz *m);

#endif