# Documentação da Matriz Encadeada

## 1. Estruturas de dados (`struct`)

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

- `inicio`: aponta para o primeiro nó da matriz, localizado na posição inicial.
- `linhas`: registra a quantidade de linhas existentes.
- `colunas`: registra a quantidade de colunas existentes.

O uso dessas estruturas permite representar uma matriz sem utilizar um vetor bidimensional convencional, possibilitando acessar os elementos por meio dos ponteiros entre os nós.

## 2. Descrição das funções

### `matrizVazia()`

Verifica se a matriz não foi inicializada ou se não possui nós. Retorna `1` quando a matriz está vazia ou é nula e `0` quando possui elementos.

### `criarNo()`

Aloca memória para um novo nó, atribui o valor inicial informado e inicializa os quatro ponteiros com `NULL`. Caso a alocação falhe, retorna `NULL`.

### `criarMatriz()`

Cria uma matriz com a quantidade de linhas e colunas especificada. Inicializa os nós com valor zero e estabelece as conexões horizontais e verticais. Retorna um ponteiro para a matriz criada ou `NULL` em caso de dimensões inválidas ou falha na alocação.

### `desalocarMatriz()`

Libera a memória ocupada pelos nós e pela estrutura que representa a matriz. O percurso ocorre linha por linha, evitando liberar um mesmo nó mais de uma vez.

### `validarPosicao()`

Verifica se as coordenadas informadas pertencem aos limites da matriz. As posições são indexadas a partir de zero. Retorna `1` para uma posição válida e `0` para uma posição inválida.

### `localizarElemento()`

Localiza e retorna o endereço do nó correspondente à linha e à coluna informadas. Antes de percorrer a matriz, utiliza a validação das coordenadas. Retorna `NULL` caso a posição seja inválida ou o nó não seja encontrado.

### `acessarPosicao()`

Disponibiliza o acesso a um elemento por suas coordenadas. Reutiliza `localizarElemento()` para encontrar o nó, evitando duplicar a lógica de percurso.

### `inserirValor()`

Insere ou substitui o valor inteiro de um elemento em uma posição específica. Utiliza `acessarPosicao()` para localizar o nó e retorna `1` em caso de sucesso ou `0` quando não consegue realizar a operação.

### `buscarValor()`

Percorre os nós da matriz para encontrar o primeiro elemento que contenha o valor informado. Retorna o endereço do nó encontrado ou `NULL` caso não exista um elemento com esse valor.

### `imprimirVizinhos()`

Localiza o elemento indicado pelas coordenadas e apresenta os valores de seus vizinhos à esquerda, à direita, acima e abaixo. Quando um vizinho não existe, informa essa condição.

### `adicionarLinha()`

Adiciona uma nova linha ao final da matriz. Cria os nós necessários e estabelece as conexões horizontais e verticais com a linha anterior. Os novos elementos são inicializados com valor zero.

### `adicionarColuna()`

Adiciona uma nova coluna ao final de cada linha. Cria os nós necessários e conecta-os horizontalmente e verticalmente aos elementos existentes. Os novos elementos são inicializados com valor zero.

### `removerLinha()`

Remove a linha indicada pelo índice, atualizando os ponteiros dos nós das linhas adjacentes. Retorna `1` quando a remoção é realizada e `0` quando o índice é inválido ou a operação não pode ser concluída.

### `removerColuna()`

Remove a coluna indicada pelo índice, ajustando as conexões horizontais dos nós restantes. Retorna `1` em caso de sucesso e `0` quando a coluna informada é inválida ou a operação não pode ser realizada.

### `imprimirMatriz()`

Percorre a matriz linha por linha e apresenta os valores de seus elementos na tela. Caso a matriz esteja vazia ou não tenha sido inicializada, informa essa condição.

## 3. Tratamento da redundância de código

Durante a organização das funções, buscou-se centralizar operações comuns e reutilizar funções já implementadas, reduzindo a repetição de código e facilitando a manutenção.

As principais decisões foram:

- **`matrizVazia()`** centraliza a verificação de uma matriz vazia ou não inicializada. Essa verificação é reutilizada por funções como `validarPosicao()`, `buscarValor()` e `imprimirMatriz()`.

- **`validarPosicao()`** concentra a verificação dos limites das coordenadas, evitando que cada operação precise implementar novamente as mesmas condições de validade.

- **`localizarElemento()`** centraliza o percurso necessário para encontrar um nó por linha e coluna. Dessa forma, as operações que precisam localizar um elemento podem reutilizar essa função.

- **`acessarPosicao()`** utiliza `localizarElemento()` em vez de repetir o percurso pela matriz.

- **`inserirValor()`** utiliza `acessarPosicao()` para localizar o elemento antes de alterar seu valor, eliminando a necessidade de implementar novamente a busca por coordenadas.

- **`imprimirVizinhos()`** também utiliza `acessarPosicao()`, reutilizando a lógica de localização e concentrando-se apenas na apresentação dos valores dos vizinhos.

- **`criarNo()`** centraliza a alocação e a inicialização dos nós. Essa função é utilizada na criação e na expansão da matriz, evitando repetir a inicialização dos quatro ponteiros em diferentes operações.

As funções `buscarValor()`, `imprimirMatriz()` e `desalocarMatriz()` mantêm percursos próprios porque possuem objetivos diferentes: localizar um valor, apresentar os elementos e liberar a memória, respectivamente. Embora compartilhem uma estrutura de percurso semelhante, reuni-las em uma única função não traria necessariamente uma redução de complexidade.

## 4. Considerações sobre a organização

A separação entre `matriz.h` e `matriz.c` permite organizar as declarações e as implementações de maneira independente.

O arquivo `matriz.h` contém as estruturas de dados e os protótipos das funções, permitindo que outros arquivos conheçam a interface disponibilizada pelo módulo.

O arquivo `matriz.c` contém a implementação das operações de criação, acesso, busca, inserção, impressão, expansão, remoção e liberação da matriz.

Essa organização favorece a modularização, a reutilização das funções e a manutenção do código.
