
#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "linked-list.h"
#include "aeroport.h"


void insertarAeropuerto(List **tablaHash, struct InformacionVuelo *infoVuelo);

int getHashValue(char *cadena);

void deleteTable(List **table);

List **crearHashTable();

void hashTable2RBTree(List **hashTable, RBTree *tree);

#endif /* HASHTABLE_H */

