#include "matriz.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>


int matrizVazia(Matriz *m) {
    return m == NULL || m->inicio == NULL;
}


No *criarNo(int valor) {
    No *novo = malloc(sizeof *novo);
    if (novo == NULL)
        return NULL;

    novo->valor = valor;
    novo->esquerda = NULL;
    novo->direita = NULL;
    novo->cima = NULL;
    novo->baixo = NULL;
    return novo;
}


/* Cria uma linha com `colunas` nos ligados horizontalmente.
   Retorna o primeiro no, ou NULL (sem deixar memoria alocada) se falhar. */
static No *criarLinha(int colunas) {
    No *primeiro = NULL;
    No *ultimo = NULL;

    for (int j = 0; j < colunas; j++) {
        No *novo = criarNo(0);
        if (novo == NULL) {
            while (primeiro != NULL) {
                No *proximo = primeiro->direita;
                free(primeiro);
                primeiro = proximo;
            }
            return NULL;
        }

        if (ultimo != NULL) {
            ultimo->direita = novo;
            novo->esquerda = ultimo;
        } else {
            primeiro = novo;
        }
        ultimo = novo;
    }

    return primeiro;
}

/* Liga verticalmente duas linhas vizinhas, no a no. */
static void ligarLinhas(No *acima, No *abaixo) {
    while (acima != NULL && abaixo != NULL) {
        acima->baixo = abaixo;
        abaixo->cima = acima;
        acima = acima->direita;
        abaixo = abaixo->direita;
    }
}

Matriz *criarMatriz(int linhas, int colunas) {
    if (linhas <= 0 || colunas <= 0)
        return NULL;

    Matriz *m = malloc(sizeof *m);
    if (m == NULL)
        return NULL;

    m->inicio = NULL;
    m->linhas = 0;
    m->colunas = colunas;

    No *ultimaLinha = NULL;

    for (int i = 0; i < linhas; i++) {
        No *nova = criarLinha(colunas);
        if (nova == NULL) {
            desalocarMatriz(m);
            return NULL;
        }

        if (ultimaLinha != NULL)
            ligarLinhas(ultimaLinha, nova);
        else
            m->inicio = nova;

        ultimaLinha = nova;
        m->linhas++;
    }

    return m;
}

void desalocarMatriz(Matriz *m) {
    if (m == NULL)
        return;

    No *linha = m->inicio;
    while (linha != NULL) {
        No *proximaLinha = linha->baixo;
        No *atual = linha;

        while (atual != NULL) {
            No *proximo = atual->direita;
            free(atual);
            atual = proximo;
        }

        linha = proximaLinha;
    }

    free(m);
}

int validarPosicao(Matriz *m, int linha, int coluna) {
    if (matrizVazia(m))
        return 0;

    return linha >= 0 && coluna >= 0 &&
           linha < m->linhas && coluna < m->colunas;
}

No *localizarElemento(Matriz *m, int linha, int coluna) {
    if (!validarPosicao(m, linha, coluna))
        return NULL;

    No *atual = m->inicio;

    for (int i = 0; i < linha; i++) {
        if (atual == NULL)
            return NULL;
        atual = atual->baixo;
    }

    for (int j = 0; j < coluna; j++) {
        if (atual == NULL)
            return NULL;
        atual = atual->direita;
    }

    return atual;
}

No *acessarPosicao(Matriz *m, int linha, int coluna) {
    return localizarElemento(m, linha, coluna);
}

int inserirValor(Matriz *m, int linha, int coluna, int valor) {
    No *atual = acessarPosicao(m, linha, coluna);
    if (atual == NULL)
        return 0;

    atual->valor = valor;
    return 1;
}


No *buscarValor(Matriz *m, int valor) {
    if (matrizVazia(m))
        return NULL;

    for (No *linha = m->inicio; linha != NULL; linha = linha->baixo) {
        for (No *atual = linha; atual != NULL; atual = atual->direita) {
            if (atual->valor == valor)
                return atual;
        }
    }

    return NULL;
}


void imprimirVizinhos(Matriz *m, int linha, int coluna) {
    No *atual = acessarPosicao(m, linha, coluna);
    if (atual == NULL) {
        printf("Posicao invalida.\n");
        return;
    }

    if (atual->esquerda != NULL)
        printf("Esquerda: %d\n", atual->esquerda->valor);
    else
        printf("Esquerda: inexistente\n");

    if (atual->direita != NULL)
        printf("Direita: %d\n", atual->direita->valor);
    else
        printf("Direita: inexistente\n");

    if (atual->cima != NULL)
        printf("Cima: %d\n", atual->cima->valor);
    else
        printf("Cima: inexistente\n");

    if (atual->baixo != NULL)
        printf("Baixo: %d\n", atual->baixo->valor);
    else
        printf("Baixo: inexistente\n");
}

void adicionarLinha(Matriz *m) {
    if (m == NULL || m->colunas <= 0)
        return;

    No *nova = criarLinha(m->colunas);
    if (nova == NULL)
        return;

    No *ultima = localizarElemento(m, m->linhas - 1, 0);
    if (ultima != NULL)
        ligarLinhas(ultima, nova);
    else
        m->inicio = nova;

    m->linhas++;
}

void adicionarColuna(Matriz *m) {
    if (m == NULL)
        return;

    if (m->linhas == 0) {
        m->colunas++;
        return;
    }

    No *primeiroNovo = NULL;
    No *ultimoNovo = NULL;

    for (int i = 0; i < m->linhas; i++) {
        No *novo = criarNo(0);
        if (novo == NULL) {
            No *atual = primeiroNovo;
            while (atual != NULL) {
                No *abaixo = atual->baixo;
                free(atual);
                atual = abaixo;
            }
            return;
        }

        if (ultimoNovo != NULL) {
            ultimoNovo->baixo = novo;
            novo->cima = ultimoNovo;
        } else {
            primeiroNovo = novo;
        }

        ultimoNovo = novo;
    }

    No *linha = m->inicio;
    No *novo = primeiroNovo;

    while (linha != NULL && novo != NULL) {
        No *ultimo = linha;
        while (ultimo->direita != NULL)
            ultimo = ultimo->direita;

        ultimo->direita = novo;
        novo->esquerda = ultimo;

        linha = linha->baixo;
        novo = novo->baixo;
    }

    m->colunas++;
}


int removerLinha(Matriz *m, int linha) {
    if (m == NULL || linha < 0 || linha >= m->linhas)
        return 0;

    No *primeiro = localizarElemento(m, linha, 0);
    if (primeiro == NULL)
        return 0;

    No *abaixo = primeiro->baixo;
    No *atual = primeiro;

    while (atual != NULL) {
        No *proximo = atual->direita;

        if (atual->cima != NULL)
            atual->cima->baixo = atual->baixo;

        if (atual->baixo != NULL)
            atual->baixo->cima = atual->cima;

        free(atual);
        atual = proximo;
    }

    if (linha == 0)
        m->inicio = abaixo;

    m->linhas--;

    if (m->linhas == 0) {
        m->inicio = NULL;
        m->colunas = 0;
    }

    return 1;
}


int removerColuna(Matriz *m, int coluna) {
    if (m == NULL || coluna < 0 || coluna >= m->colunas ||
        m->linhas == 0)
        return 0;

    if (m->colunas == 1) {
        No *linha = m->inicio;
        while (linha != NULL) {
            No *abaixo = linha->baixo;
            free(linha);
            linha = abaixo;
        }

        m->inicio = NULL;
        m->linhas = 0;
        m->colunas = 0;
        return 1;
    }

    No *atual = localizarElemento(m, 0, coluna);
    if (atual == NULL)
        return 0;

    if (coluna == 0)
        m->inicio = atual->direita;

    while (atual != NULL) {
        No *abaixo = atual->baixo;
        No *esquerda = atual->esquerda;
        No *direita = atual->direita;

        if (esquerda != NULL)
            esquerda->direita = direita;

        if (direita != NULL)
            direita->esquerda = esquerda;

        free(atual);
        atual = abaixo;
    }

    m->colunas--;
    return 1;
}


void imprimirMatriz(Matriz *m) {
    if (matrizVazia(m)) {
        printf("Matriz vazia.\n");
        return;
    }

    for (No *linha = m->inicio; linha != NULL; linha = linha->baixo) {
        for (No *atual = linha; atual != NULL; atual = atual->direita)
            printf("%d ", atual->valor);

        printf("\n");
    }
}

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
/* Interface com o usuario: leitura segura de dados                   */
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
Matriz *lerECriarMatriz(void) {
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
/* Menu interativo: le a opcao e chama as funcoes acima               */
/* ------------------------------------------------------------------ */

/* Executa o menu ate o usuario escolher sair. Devolve a matriz final
   (pode ser outra, se a original ficou vazia e foi recriada) ou NULL
   se a entrada terminar durante a recriacao. */
Matriz *menu(Matriz *m) {
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

    return m;
}