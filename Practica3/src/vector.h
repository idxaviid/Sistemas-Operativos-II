
#ifndef VECTOR_H
#define VECTOR_H

typedef struct Vector {
    size_t numElementos;
    char **array;
} Vector;


void inicializarVector(Vector *string_array);
void addElemento(Vector *string_array, char *to_add);
void freeVector(Vector *string_array);

#endif	// VECTOR_H

