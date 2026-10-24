#include "matriz.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define TAM_BUFFER 64

enum {
    OP_IMPRIMIR = 1,
    OP_INSERIR,
    OP_ACESSAR,
    OP_BUSCAR,
    OP_VIZINHOS,
    OP_ADD_LINHA,
    OP_ADD_COLUNA,
    OP_REM_LINHA,
    OP_REM_COLUNA,
    OP_SAIR
};

/* ------------------------------------------------------------------ */
/* Leitura segura de dados (o matriz.c nao trata entrada do usuario)  */
/* ------------------------------------------------------------------ */

/* Le um inteiro, repetindo a pergunta ate a entrada ser valida.
   Retorna 0 apenas se a entrada terminar (EOF). */
static int lerInteiro(const char *mensagem, int *valor) {
    char buffer[TAM_BUFFER];

    while (1) {
        printf("%s", mensagem);

        if (fgets(buffer, sizeof buffer, stdin) == NULL)
            return 0;

        if (strchr(buffer, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Entrada muito longa. Tente novamente.\n");
            continue;
        }

        char *fim;
        errno = 0;
        long numero = strtol(buffer, &fim, 10);

        while (isspace((unsigned char)*fim))
            fim++;

        if (fim == buffer || *fim != '\0' || errno == ERANGE ||
            numero < INT_MIN || numero > INT_MAX) {
            printf("Entrada invalida. Digite um numero inteiro.\n");
            continue;
        }

        *valor = (int)numero;
        return 1;
    }
}

/* Le um indice em [0, limite - 1], repetindo ate ser valido. */
static int lerIndice(const char *rotulo, int limite, int *indice) {
    char mensagem[TAM_BUFFER];
    snprintf(mensagem, sizeof mensagem, "%s (0 a %d): ", rotulo, limite - 1);

    while (lerInteiro(mensagem, indice)) {
        if (*indice >= 0 && *indice < limite)
            return 1;

        printf("%s inexistente. Escolha um valor entre 0 e %d.\n",
               rotulo, limite - 1);
    }

    return 0;
}

static int lerPosicao(Matriz *m, int *linha, int *coluna) {
    return lerIndice("Linha", m->linhas, linha) &&
           lerIndice("Coluna", m->colunas, coluna);
}

/* Pede as dimensoes ate criar a matriz. Retorna NULL apenas em EOF. */
static Matriz *lerECriarMatriz(void) {
    Matriz *m = NULL;

    while (m == NULL) {
        int linhas, colunas;

        if (!lerInteiro("Quantidade de linhas: ", &linhas) ||
            !lerInteiro("Quantidade de colunas: ", &colunas))
            return NULL;

        m = criarMatriz(linhas, colunas);
        if (m == NULL)
            printf("Nao foi possivel criar a matriz. "
                   "Use valores maiores que zero.\n");
    }

    printf("Matriz %dx%d criada com sucesso.\n", m->linhas, m->colunas);
    return m;
}

static void exibirMenu(void) {
    printf("\n===== MATRIZ ENCADEADA =====\n");
    printf(" 1. Imprimir matriz\n");
    printf(" 2. Inserir valor\n");
    printf(" 3. Acessar posicao\n");
    printf(" 4. Buscar valor\n");
    printf(" 5. Imprimir vizinhos\n");
    printf(" 6. Adicionar linha\n");
    printf(" 7. Adicionar coluna\n");
    printf(" 8. Remover linha\n");
    printf(" 9. Remover coluna\n");
    printf("10. Sair\n");
}

/* ------------------------------------------------------------------ */
/* Programa principal: le a opcao e chama as funcoes do matriz.c      */
/* ------------------------------------------------------------------ */

int main(void) {
    Matriz *m = lerECriarMatriz();
    int opcao = 0, linha, coluna, valor;
    No *no;

    while (m != NULL && opcao != OP_SAIR) {
        exibirMenu();
        if (!lerInteiro("Opcao: ", &opcao))
            opcao = OP_SAIR;

        switch (opcao) {
            case OP_IMPRIMIR:
                imprimirMatriz(m);
                break;

            case OP_INSERIR:
                if (lerPosicao(m, &linha, &coluna) &&
                    lerInteiro("Valor a inserir: ", &valor)) {
                    if (inserirValor(m, linha, coluna, valor))
                        printf("Valor %d inserido em (%d, %d).\n",
                               valor, linha, coluna);
                    else
                        printf("Nao foi possivel inserir o valor.\n");
                }
                break;

            case OP_ACESSAR:
                if (lerPosicao(m, &linha, &coluna)) {
                    no = acessarPosicao(m, linha, coluna);
                    if (no != NULL)
                        printf("Valor em (%d, %d): %d\n",
                               linha, coluna, no->valor);
                    else
                        printf("Posicao invalida.\n");
                }
                break;

            case OP_BUSCAR:
                if (lerInteiro("Valor a buscar: ", &valor)) {
                    if (buscarValor(m, valor) != NULL)
                        printf("Valor %d encontrado.\n", valor);
                    else
                        printf("Valor %d nao encontrado.\n", valor);
                }
                break;

            case OP_VIZINHOS:
                if (lerPosicao(m, &linha, &coluna))
                    imprimirVizinhos(m, linha, coluna);
                break;

            case OP_ADD_LINHA:
                adicionarLinha(m);
                printf("Dimensoes atuais: %dx%d\n", m->linhas, m->colunas);
                break;

            case OP_ADD_COLUNA:
                adicionarColuna(m);
                printf("Dimensoes atuais: %dx%d\n", m->linhas, m->colunas);
                break;

            case OP_REM_LINHA:
                if (lerIndice("Linha", m->linhas, &linha)) {
                    if (removerLinha(m, linha))
                        printf("Linha %d removida.\n", linha);
                    else
                        printf("Nao foi possivel remover a linha.\n");
                }
                break;

            case OP_REM_COLUNA:
                if (lerIndice("Coluna", m->colunas, &coluna)) {
                    if (removerColuna(m, coluna))
                        printf("Coluna %d removida.\n", coluna);
                    else
                        printf("Nao foi possivel remover a coluna.\n");
                }
                break;

            case OP_SAIR:
                printf("Encerrando e liberando memoria...\n");
                break;

            default:
                printf("Opcao invalida. Escolha um numero de 1 a 10.\n");
        }

        /* Se a ultima linha/coluna foi removida, a matriz ficou sem nos:
           libera a estrutura e pede novas dimensoes. */
        if (opcao != OP_SAIR && matrizVazia(m)) {
            printf("\nA matriz ficou vazia. Crie uma nova.\n");
            desalocarMatriz(m);
            m = lerECriarMatriz();
        }
    }

    if (m == NULL)
        return EXIT_FAILURE;

    desalocarMatriz(m);
    return EXIT_SUCCESS;
}