/*
 *  Descripcion: Librería que contiene funciones para añadir retrasos y aeropuerto.
 */

#ifndef AEROPUERTO_H_
#define AEROPUERTO_H_

#include "red-black-tree.h"
#include "linked-list.h"

typedef struct InformacionVuelo {
    char *aeropuertoOrigen;
    char *aeropuertoDestino;
    int retrasoVuelo;
    int diaVuelo;
} InformacionVuelo;


void add2Hash(List *aeropuertoOrigen, struct InformacionVuelo *infoVuelo);

void addInfoRetraso(RBData *treeData, ListData *data);

void addInfoVuelo(RBTree *tree, ListData *data);

float mitjana_diaria(RBTree *tree, char *aeroportOrigen, char *aeroportDesti, int diaVol);

void printAeroport(RBData *aeroport);

#endif /* AEROPUERTO_H_ */

