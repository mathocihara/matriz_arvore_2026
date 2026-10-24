#include "matriz.h"

int main(void) {
    Matriz *m = lerECriarMatriz();
    if (m == NULL)
        return EXIT_FAILURE;

    m = menu(m);
    if (m == NULL)
        return EXIT_FAILURE;

    desalocarMatriz(m);
    return EXIT_SUCCESS;
}