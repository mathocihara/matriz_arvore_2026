# Documentação da Matriz Encadeada

Trabalho 1 de Algoritmos e Estrutura de Dados III: simulação de uma matriz bidimensional de inteiros por meio de listas encadeadas, em que cada elemento aponta para seus vizinhos à esquerda, à direita, acima e abaixo.

## Sumário

1. [Organização dos arquivos](#1-organização-dos-arquivos)
2. [Como compilar e executar](#2-como-compilar-e-executar)
3. [Estruturas de dados (`struct`)](#3-estruturas-de-dados-struct)
4. [Descrição das funções de `matriz.c`](#4-descrição-das-funções-de-matrizc)
5. [Programa principal e menu (`main.c`)](#5-programa-principal-e-menu-mainc)
6. [Tratamento da redundância de código](#6-tratamento-da-redundância-de-código)
7. [Desafios e dificuldades](#7-desafios-e-dificuldades)
8. [Considerações sobre a organização](#8-considerações-sobre-a-organização)

## 1. Organização dos arquivos

| Arquivo | Conteúdo |
| --- | --- |
| `matriz.h` | Estruturas de dados e protótipos das funções. |
| `matriz.c` | Implementação das operações sobre a matriz encadeada. |
| `main.c` | Função `main()`, leitura segura de dados e menu interativo. |

## 2. Como compilar e executar

Com os três arquivos na mesma pasta:

```
gcc -Wall -Wextra main.c matriz.c -o matriz
```

Para executar:

- Linux ou macOS: `./matriz`
- Windows (PowerShell): `.\matriz`

Para verificar vazamentos e acessos inválidos à memória (Linux, macOS ou WSL), pode-se compilar com o AddressSanitizer:

```
gcc -Wall -Wextra -g -fsanitize=address,undefined main.c matriz.c -o matriz
```

## 3. Estruturas de dados (`struct`)

O projeto utiliza duas estruturas principais para representar uma matriz bidimensional por meio de listas encadeadas.

### Estrutura `No`

A estrutura `No` representa cada elemento da matriz. Ela contém:

- `valor`: armazena o valor inteiro do elemento.
- `esquerda`: aponta para o nó imediatamente à esquerda.
- `direita`: aponta para o nó imediatamente à direita.
- `cima`: aponta para o nó imediatamente acima.
- `baixo`: aponta para o nó imediatamente abaixo.

Esses quatro ponteiros permitem percorrer a matriz horizontal e verticalmente. Quando não existe um vizinho em determinada direção, o ponteiro correspondente recebe `NULL`.

### Estrutura `Matriz`

A estrutura `Matriz` representa a matriz encadeada como um todo. Ela contém:

- `inicio`: aponta para o primeiro nó da matriz, localizado na posição (0,0).
- `linhas`: registra a quantidade de linhas existentes.
- `colunas`: registra a quantidade de colunas existentes.

O uso dessas estruturas permite representar uma matriz sem utilizar um vetor bidimensional convencional, possibilitando acessar os elementos por meio dos ponteiros entre os nós.

## 4. Descrição das funções de `matriz.c`

### `matrizVazia()`

Verifica se a matriz não foi inicializada ou se não possui nós. Retorna `1` quando a matriz está vazia ou é nula e `0` quando possui elementos.

### `criarNo()`

Aloca memória para um novo nó, atribui o valor inicial informado e inicializa os quatro ponteiros com `NULL`. Caso a alocação falhe, retorna `NULL`.

### `criarMatriz()`

Cria uma matriz com a quantidade de linhas e colunas especificada. Inicializa os nós com valor zero e estabelece as conexões horizontais e verticais. Retorna um ponteiro para a matriz criada ou `NULL` em caso de dimensões inválidas ou falha na alocação. Se a alocação falhar no meio da criação, a memória já alocada é liberada antes do retorno.

### `desalocarMatriz()`

Libera a memória ocupada pelos nós e pela estrutura que representa a matriz. O percurso ocorre linha por linha, evitando liberar um mesmo nó mais de uma vez.

### `validarPosicao()`

Verifica se as coordenadas informadas pertencem aos limites da matriz. As posições são indexadas a partir de zero. Retorna `1` para uma posição válida e `0` para uma posição inválida.

### `localizarElemento()`

Localiza e retorna o endereço do nó correspondente à linha e à coluna informadas. Antes de percorrer a matriz, utiliza a validação das coordenadas. O percurso desce `linha` vezes pelo ponteiro `baixo` e avança `coluna` vezes pelo ponteiro `direita`. Retorna `NULL` caso a posição seja inválida ou o nó não seja encontrado.

### `acessarPosicao()`

Disponibiliza o acesso a um elemento por suas coordenadas. Reutiliza `localizarElemento()` para encontrar o nó, evitando duplicar a lógica de percurso. Retorna o nó ou `NULL`, conforme exigido pelo enunciado.

### `inserirValor()`

Insere ou substitui o valor inteiro de um elemento em uma posição específica. Utiliza `acessarPosicao()` para localizar o nó e retorna `1` em caso de sucesso ou `0` quando a posição está fora dos limites da matriz.

### `buscarValor()`

Percorre os nós da matriz, linha por linha, para encontrar o primeiro elemento que contenha o valor informado. Retorna o endereço do nó encontrado ou `NULL` caso não exista um elemento com esse valor.

### `imprimirVizinhos()`

Localiza o elemento indicado pelas coordenadas e apresenta os valores de seus vizinhos à esquerda, à direita, acima e abaixo. Quando um vizinho não existe, informa essa condição. Se a posição for inválida, exibe uma mensagem de erro.

### `adicionarLinha()`

Adiciona uma nova linha ao final da matriz. Cria os nós necessários e estabelece as conexões horizontais e verticais com a linha anterior. Os novos elementos são inicializados com valor zero.

### `adicionarColuna()`

Adiciona uma nova coluna ao final de cada linha. Cria os nós necessários e conecta-os horizontalmente e verticalmente aos elementos existentes. Os novos elementos são inicializados com valor zero.

### `removerLinha()`

Remove a linha indicada pelo índice, atualizando os ponteiros dos nós das linhas adjacentes. Quando a linha removida é a primeira, o ponteiro `inicio` passa a apontar para a linha seguinte. Retorna `1` quando a remoção é realizada e `0` quando o índice é inválido ou a operação não pode ser concluída. Se a última linha for removida, a matriz fica vazia (`inicio` igual a `NULL`, com `linhas` e `colunas` iguais a zero).

### `removerColuna()`

Remove a coluna indicada pelo índice, ajustando as conexões horizontais dos nós restantes. Quando a coluna removida é a primeira (índice `0`), o ponteiro `inicio` é atualizado para o nó à direita do nó `(0,0)` antes da liberação da memória; sem esse ajuste, `inicio` ficaria apontando para um nó já liberado. Se a matriz possui apenas uma coluna, todos os nós são liberados e a matriz fica vazia. Retorna `1` em caso de sucesso e `0` quando a coluna informada é inválida ou a operação não pode ser realizada.

### `imprimirMatriz()`

Percorre a matriz linha por linha e apresenta os valores de seus elementos na tela. Caso a matriz esteja vazia ou não tenha sido inicializada, informa essa condição.

## 5. Programa principal e menu (`main.c`)

O arquivo `main.c` conecta as funções de `matriz.c` à interação com o usuário. Ao iniciar, solicita a quantidade de linhas e de colunas, cria a matriz com `criarMatriz()` e exibe um menu que permanece ativo até que o usuário escolha sair. Ao encerrar, a memória é liberada com `desalocarMatriz()`.

Os índices de linha e coluna são sempre indexados a partir de zero.

### Opções do menu

| Opção | Operação | Função utilizada |
| --- | --- | --- |
| 1 | Imprimir matriz | `imprimirMatriz()` |
| 2 | Inserir valor em uma posição | `inserirValor()` |
| 3 | Acessar o valor de uma posição | `acessarPosicao()` |
| 4 | Buscar um valor | `buscarValor()` |
| 5 | Imprimir os vizinhos de uma posição | `imprimirVizinhos()` |
| 6 | Adicionar linha | `adicionarLinha()` |
| 7 | Adicionar coluna | `adicionarColuna()` |
| 8 | Remover linha | `removerLinha()` |
| 9 | Remover coluna | `removerColuna()` |
| 10 | Sair e liberar a memória | `desalocarMatriz()` |

A cada operação o programa exibe uma mensagem com o resultado. Opções inválidas e posições inexistentes são informadas ao usuário, que pode tentar novamente.

### Funções auxiliares de leitura

- **`lerInteiro()`**: lê uma linha com `fgets()` e a converte com `strtol()`. Rejeita letras, caracteres extras após o número, valores fora do intervalo de `int` e linhas longas demais, repetindo a pergunta até receber uma entrada válida. Retorna `0` apenas se a entrada terminar (EOF).
- **`lerIndice()`**: usa `lerInteiro()` e repete a pergunta até que o valor esteja entre `0` e o limite informado, exibindo o intervalo aceito.
- **`lerPosicao()`**: lê linha e coluna reutilizando `lerIndice()` com os limites atuais da matriz.
- **`lerECriarMatriz()`**: solicita as dimensões e chama `criarMatriz()`, repetindo o processo se as dimensões forem inválidas ou a alocação falhar.
- **`descartarLinha()`**: descarta o restante de uma linha digitada quando ela excede o tamanho do buffer.

### Funções de operação

- **`opInserir()`**, **`opAcessar()`**, **`opBuscar()`** e **`opVizinhos()`**: leem os dados necessários, chamam a função correspondente de `matriz.c` e exibem o resultado.
- **`opBuscar()`** informa também a posição `(linha, coluna)` do valor encontrado, obtida por meio de **`obterCoordenadas()`**, que percorre a matriz comparando o endereço do nó retornado por `buscarValor()`.
- **`opAdicionar()`**: recebe por ponteiro de função `adicionarLinha()` ou `adicionarColuna()` e confirma o sucesso comparando a quantidade de linhas (ou colunas) antes e depois da operação.
- **`opRemover()`**: recebe por ponteiro de função `removerLinha()` ou `removerColuna()`, lê o índice válido e informa o resultado.
- **`exibirMenu()`**: apresenta as opções disponíveis.

### Matriz vazia

Quando a última linha ou coluna é removida, a matriz deixa de ter nós. Nesse caso, o `main()` avisa o usuário, libera a estrutura e solicita novas dimensões com `lerECriarMatriz()`, permitindo que o programa continue sem operar sobre uma matriz sem elementos.

## 6. Tratamento da redundância de código

Durante a organização das funções, buscou-se centralizar operações comuns e reutilizar funções já implementadas, reduzindo a repetição de código e facilitando a manutenção.

As principais decisões em `matriz.c` foram:

- **`matrizVazia()`** centraliza a verificação de uma matriz vazia ou não inicializada. Essa verificação é reutilizada por funções como `validarPosicao()`, `buscarValor()` e `imprimirMatriz()`.

- **`validarPosicao()`** concentra a verificação dos limites das coordenadas, evitando que cada operação precise implementar novamente as mesmas condições de validade.

- **`localizarElemento()`** centraliza o percurso necessário para encontrar um nó por linha e coluna. Dessa forma, as operações que precisam localizar um elemento podem reutilizar essa função.

- **`acessarPosicao()`** utiliza `localizarElemento()` em vez de repetir o percurso pela matriz.

- **`inserirValor()`** utiliza `acessarPosicao()` para localizar o elemento antes de alterar seu valor, eliminando a necessidade de implementar novamente a busca por coordenadas.

- **`imprimirVizinhos()`** também utiliza `acessarPosicao()`, reutilizando a lógica de localização e concentrando-se apenas na apresentação dos valores dos vizinhos.

- **`criarNo()`** centraliza a alocação e a inicialização dos nós. Essa função é utilizada na criação e na expansão da matriz, evitando repetir a inicialização dos quatro ponteiros em diferentes operações.

As funções `buscarValor()`, `imprimirMatriz()` e `desalocarMatriz()` mantêm percursos próprios porque possuem objetivos diferentes: localizar um valor, apresentar os elementos e liberar a memória, respectivamente. Embora compartilhem uma estrutura de percurso semelhante, reuni-las em uma única função não traria necessariamente uma redução de complexidade.

No `main.c`, as decisões foram:

- **`lerInteiro()`** concentra toda a leitura e a validação de números. As demais funções de leitura e de operação a reutilizam, em vez de repetir `scanf()` e verificações em cada opção do menu.

- **`lerIndice()`** é reutilizada por `lerPosicao()` (linha e coluna) e por `opRemover()`, de modo que a validação de intervalo existe em um único lugar.

- **`opAdicionar()` e `opRemover()`** tratam linhas e colunas com uma única implementação cada, recebendo a função específica por ponteiro. Isso evita ter quatro funções quase idênticas para as opções 6 a 9 do menu.

- **`lerECriarMatriz()`** é usada tanto na criação inicial quanto na recriação da matriz após ela ficar vazia.

## 7. Desafios e dificuldades

- **Manter os quatro ponteiros consistentes.** Cada inserção ou remoção de linha ou coluna exige atualizar os ponteiros `esquerda`, `direita`, `cima` e `baixo` dos nós vizinhos. Um ponteiro esquecido deixa a estrutura inconsistente e pode causar falhas apenas em operações posteriores.

- **Remoção da primeira linha ou coluna.** Remover a linha ou a coluna de índice `0` obriga a atualizar o ponteiro `inicio` da `Matriz`. Na primeira versão de `removerColuna()`, isso não era feito: ao remover a coluna `0`, `inicio` permanecia apontando para um nó já liberado (uso de memória após `free`). O problema foi identificado ao executar o programa com o AddressSanitizer e corrigido atribuindo `m->inicio = atual->direita` antes da liberação dos nós.

- **Matriz vazia.** Remover a última linha ou coluna deixa a matriz sem nós, e operações como `adicionarLinha()` não têm uma coluna de referência para recriar a estrutura. Para evitar esse estado, o `main()` detecta a matriz vazia e solicita novas dimensões.

- **Gerenciamento de memória.** Foi necessário garantir que todo nó alocado fosse liberado exatamente uma vez, inclusive quando a alocação falha no meio da criação da matriz e na remoção de linhas e colunas.

- **Entrada de dados do usuário.** O uso direto de `scanf()` falha com entradas não numéricas, deixando caracteres no buffer. A leitura com `fgets()` e `strtol()` resolveu o problema, mas exigiu tratar linhas longas, caracteres extras e fim da entrada.

- **Reduzir repetição.** Operações de linha e de coluna são muito parecidas, mas diferem nos ponteiros utilizados. No `main.c`, o uso de ponteiros de função permitiu compartilhar a lógica de leitura e de mensagens entre as duas.

## 8. Considerações sobre a organização

A separação entre `matriz.h`, `matriz.c` e `main.c` permite organizar as declarações, as implementações e a interação com o usuário de maneira independente.

O arquivo `matriz.h` contém as estruturas de dados e os protótipos das funções, permitindo que outros arquivos conheçam a interface disponibilizada pelo módulo.

O arquivo `matriz.c` contém a implementação das operações de criação, acesso, busca, inserção, impressão, expansão, remoção e liberação da matriz.

O arquivo `main.c` contém apenas a interface com o usuário: leitura e validação de dados, menu e chamadas às funções do módulo. Nenhuma operação sobre os nós é feita diretamente nele, exceto a consulta de coordenadas em `obterCoordenadas()`.

Essa organização favorece a modularização, a reutilização das funções e a manutenção do código.