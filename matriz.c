#include "matriz.h"


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
        No *primeiroNovo = NULL;
        No *ultimoNovo = NULL;

        for (int j = 0; j < colunas; j++) {
            No *novo = criarNo(0);
            if (novo == NULL) {
                No *atual = primeiroNovo;
                while (atual != NULL) {
                    No *proximo = atual->direita;
                    free(atual);
                    atual = proximo;
                }
                desalocarMatriz(m);
                return NULL;
            }

            if (ultimoNovo != NULL) {
                ultimoNovo->direita = novo;
                novo->esquerda = ultimoNovo;
            } else {
                primeiroNovo = novo;
            }
            ultimoNovo = novo;
        }

        if (ultimaLinha != NULL) {
            No *acima = ultimaLinha;
            No *novo = primeiroNovo;

            while (acima != NULL && novo != NULL) {
                acima->baixo = novo;
                novo->cima = acima;
                acima = acima->direita;
                novo = novo->direita;
            }
        } else {
            m->inicio = primeiroNovo;
        }

        ultimaLinha = primeiroNovo;
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
    if (m == NULL || m->colunas < 0)
        return;

    No *primeiroNovo = NULL;
    No *ultimoNovo = NULL;
    No *linhaAcima = m->inicio;

    if (linhaAcima != NULL) {
        for (int i = 1; i < m->linhas; i++)
            linhaAcima = linhaAcima->baixo;
    }

    for (int j = 0; j < m->colunas; j++) {
        No *novo = criarNo(0);
        if (novo == NULL) {
            No *atual = primeiroNovo;
            while (atual != NULL) {
                No *proximo = atual->direita;
                free(atual);
                atual = proximo;
            }
            return;
        }

        if (ultimoNovo != NULL) {
            ultimoNovo->direita = novo;
            novo->esquerda = ultimoNovo;
        } else {
            primeiroNovo = novo;
        }

        if (linhaAcima != NULL) {
            novo->cima = linhaAcima;
            linhaAcima->baixo = novo;
            linhaAcima = linhaAcima->direita;
        }

        ultimoNovo = novo;
    }

    if (m->inicio == NULL)
        m->inicio = primeiroNovo;

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

void MENU(Matriz *matriz) {
    int opcao;
    int linha, coluna, valor;
    No *elemento;

    do {
        printf("\n========== MENU DA MATRIZ ==========\n");
        printf("1. Imprimir matriz\n");
        printf("2. Inserir valor\n");
        printf("3. Acessar posicao\n");
        printf("4. Buscar valor\n");
        printf("5. Imprimir vizinhos\n");
        printf("6. Adicionar linha\n");
        printf("7. Adicionar coluna\n");
        printf("8. Remover linha\n");
        printf("9. Remover coluna\n");
        printf("10. Sair\n");
        printf("====================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                imprimirMatriz(matriz);
                break;

            case 2:
                printf("Digite a linha: ");
                scanf("%d", &linha);

                printf("Digite a coluna: ");
                scanf("%d", &coluna);

                printf("Digite o valor: ");
                scanf("%d", &valor);

                if (inserirValor(matriz, linha, coluna, valor))
                    printf("Valor inserido com sucesso!\n");
                else
                    printf("Posicao invalida!\n");
                break;

            case 3:
                printf("Digite a linha: ");
                scanf("%d", &linha);

                printf("Digite a coluna: ");
                scanf("%d", &coluna);

                elemento = acessarPosicao(matriz, linha, coluna);

                if (elemento != NULL)
                    printf("Valor encontrado: %d\n", elemento->valor);
                else
                    printf("Posicao invalida!\n");
                break;

            case 4:
                printf("Digite o valor que deseja buscar: ");
                scanf("%d", &valor);

                elemento = buscarValor(matriz, valor);

                if (elemento != NULL)
                    printf("Valor encontrado na matriz!\n");
                else
                    printf("Valor nao encontrado!\n");
                break;

            case 5:
                printf("Digite a linha: ");
                scanf("%d", &linha);

                printf("Digite a coluna: ");
                scanf("%d", &coluna);

                imprimirVizinhos(matriz, linha, coluna);
                break;

            case 6:
                adicionarLinha(matriz);
                printf("Operacao de adicionar linha executada.\n");
                break;

            case 7:
                adicionarColuna(matriz);
                printf("Operacao de adicionar coluna executada.\n");
                break;

            case 8:
                printf("Digite o indice da linha que deseja remover: ");
                scanf("%d", &linha);

                if (removerLinha(matriz, linha))
                    printf("Linha removida com sucesso!\n");
                else
                    printf("Nao foi possivel remover a linha.\n");
                break;

            case 9:
                printf("Digite o indice da coluna que deseja remover: ");
                scanf("%d", &coluna);

                if (removerColuna(matriz, coluna))
                    printf("Coluna removida com sucesso!\n");
                else
                    printf("Nao foi possivel remover a coluna.\n");
                break;

            case 10:
                printf("Saindo do menu...\n");
                break;

            default:
                printf("Opcao invalida! Escolha uma opcao de 1 a 10.\n");
        }

    } while (opcao != 10);
}