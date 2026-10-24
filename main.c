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
/* Leitura segura de dados                                            */
/* ------------------------------------------------------------------ */

/* Descarta o que sobrou na linha de entrada (buffer estourado). */
static void descartarLinha(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* Le um inteiro, repetindo a pergunta ate a entrada ser valida.
   Retorna 0 apenas se a entrada terminar (EOF). */
static int lerInteiro(const char *mensagem, int *valor) {
    char buffer[TAM_BUFFER];

    while (1) {
        printf("%s", mensagem);

        if (fgets(buffer, sizeof buffer, stdin) == NULL)
            return 0;

        if (strchr(buffer, '\n') == NULL) {
            descartarLinha();
            printf("Entrada muito longa. Tente novamente.\n");
            continue;
        }

        char *fim;
        errno = 0;
        long numero = strtol(buffer, &fim, 10);

        if (fim == buffer || errno == ERANGE ||
            numero < INT_MIN || numero > INT_MAX) {
            printf("Entrada invalida. Digite um numero inteiro.\n");
            continue;
        }

        while (isspace((unsigned char)*fim))
            fim++;

        if (*fim != '\0') {
            printf("Entrada invalida. Digite apenas um numero inteiro.\n");
            continue;
        }

        *valor = (int)numero;
        return 1;
    }
}

/* Le um indice no intervalo [0, limite - 1], repetindo ate ser valido. */
static int lerIndice(const char *rotulo, int limite, int *indice) {
    char mensagem[TAM_BUFFER];
    snprintf(mensagem, sizeof mensagem, "%s (0 a %d): ", rotulo, limite - 1);

    while (1) {
        if (!lerInteiro(mensagem, indice))
            return 0;

        if (*indice >= 0 && *indice < limite)
            return 1;

        printf("%s inexistente. Escolha um valor entre 0 e %d.\n",
               rotulo, limite - 1);
    }
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

/* ------------------------------------------------------------------ */
/* Operacoes do menu                                                  */
/* ------------------------------------------------------------------ */

/* Descobre a posicao (linha, coluna) de um no percorrendo a matriz. */
static int obterCoordenadas(Matriz *m, No *alvo, int *linha, int *coluna) {
    int i = 0;

    for (No *l = m->inicio; l != NULL; l = l->baixo, i++) {
        int j = 0;
        for (No *a = l; a != NULL; a = a->direita, j++) {
            if (a == alvo) {
                *linha = i;
                *coluna = j;
                return 1;
            }
        }
    }

    return 0;
}

static void opInserir(Matriz *m) {
    int linha, coluna, valor;

    if (!lerPosicao(m, &linha, &coluna) ||
        !lerInteiro("Valor a inserir: ", &valor))
        return;

    if (inserirValor(m, linha, coluna, valor))
        printf("Valor %d inserido em (%d, %d).\n", valor, linha, coluna);
    else
        printf("Nao foi possivel inserir o valor.\n");
}

static void opAcessar(Matriz *m) {
    int linha, coluna;

    if (!lerPosicao(m, &linha, &coluna))
        return;

    No *no = acessarPosicao(m, linha, coluna);
    if (no != NULL)
        printf("Valor em (%d, %d): %d\n", linha, coluna, no->valor);
    else
        printf("Posicao invalida.\n");
}

static void opBuscar(Matriz *m) {
    int valor, linha, coluna;

    if (!lerInteiro("Valor a buscar: ", &valor))
        return;

    No *no = buscarValor(m, valor);
    if (no != NULL && obterCoordenadas(m, no, &linha, &coluna))
        printf("Valor %d encontrado em (%d, %d).\n", valor, linha, coluna);
    else
        printf("Valor %d nao encontrado.\n", valor);
}

static void opVizinhos(Matriz *m) {
    int linha, coluna;

    if (!lerPosicao(m, &linha, &coluna))
        return;

    printf("Vizinhos de (%d, %d):\n", linha, coluna);
    imprimirVizinhos(m, linha, coluna);
}

/* Serve para adicionarLinha e adicionarColuna: `contador` aponta para
   m->linhas ou m->colunas, usado para confirmar que a operacao ocorreu. */
static void opAdicionar(Matriz *m, const char *rotulo,
                        void (*adicionar)(Matriz *), const int *contador) {
    int antes = *contador;

    adicionar(m);

    if (*contador > antes)
        printf("%s adicionada. Matriz agora e %dx%d.\n",
               rotulo, m->linhas, m->colunas);
    else
        printf("Nao foi possivel adicionar a %s.\n", rotulo);
}

/* Serve para removerLinha e removerColuna. */
static void opRemover(Matriz *m, const char *rotulo, int limite,
                      int (*remover)(Matriz *, int)) {
    int indice;

    if (!lerIndice(rotulo, limite, &indice))
        return;

    if (remover(m, indice))
        printf("%s %d removida.\n", rotulo, indice);
    else
        printf("Nao foi possivel remover a %s.\n", rotulo);
}

/* ------------------------------------------------------------------ */
/* Menu                                                               */
/* ------------------------------------------------------------------ */

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

int main(void) {
    Matriz *m = lerECriarMatriz();
    if (m == NULL)
        return EXIT_FAILURE;

    int opcao;

    do {
        exibirMenu();

        if (!lerInteiro("Opcao: ", &opcao))
            opcao = OP_SAIR;

        switch (opcao) {
            case OP_IMPRIMIR:
                imprimirMatriz(m);
                break;
            case OP_INSERIR:
                opInserir(m);
                break;
            case OP_ACESSAR:
                opAcessar(m);
                break;
            case OP_BUSCAR:
                opBuscar(m);
                break;
            case OP_VIZINHOS:
                opVizinhos(m);
                break;
            case OP_ADD_LINHA:
                opAdicionar(m, "Linha", adicionarLinha, &m->linhas);
                break;
            case OP_ADD_COLUNA:
                opAdicionar(m, "Coluna", adicionarColuna, &m->colunas);
                break;
            case OP_REM_LINHA:
                opRemover(m, "Linha", m->linhas, removerLinha);
                break;
            case OP_REM_COLUNA:
                opRemover(m, "Coluna", m->colunas, removerColuna);
                break;
            case OP_SAIR:
                printf("Encerrando e liberando memoria...\n");
                break;
            default:
                printf("Opcao invalida. Escolha um numero de 1 a 10.\n");
        }

        /* Se a ultima linha/coluna foi removida, a matriz deixa de existir
           na pratica: pede novas dimensoes para continuar. */
        if (opcao != OP_SAIR && matrizVazia(m)) {
            printf("\nA matriz ficou vazia. Crie uma nova.\n");
            desalocarMatriz(m);
            m = lerECriarMatriz();
            if (m == NULL)
                return EXIT_FAILURE;
        }
    } while (opcao != OP_SAIR);

    desalocarMatriz(m);
    return EXIT_SUCCESS;
}