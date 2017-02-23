
/*
 *  Descripcion: Funcion vector.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "globals.h"

void inicializarVector(Vector *vector) {
    vector->array = (char **) malloc(sizeof (char *) * NBlock);
    vector->numElementos = 0;
}

void addElemento(Vector *vector, char *lineaAeropuerto) {
    vector->array[vector->numElementos] = (char*) malloc(sizeof(char)*(strlen(lineaAeropuerto)+1));
    strcpy(vector->array[vector->numElementos],lineaAeropuerto);

    ++vector->numElementos;
}

void freeVector(Vector *vector) {
    int i;
    for (i = 0; i < vector->numElementos; ++i) {
        free(vector->array[i]);
    }

    free(vector->array);
    free(vector);
}

