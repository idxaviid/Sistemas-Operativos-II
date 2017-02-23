#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aeroport.h"
#include "globals.h"

/**
 * Funció que mostra l'informació d'un aeroport
 * Arguments:
 * 		RBData *aeropuerto: Node aeropuert
 */
void printAeroport(RBData *aeroport) {
    printf("Info aeroport origen:%s\n", aeroport->key);

    dumpList(aeroport->aeropuertosDestino);

}

/**
 *  Transfereix dades entre dos listData
 */
void transferDataHash2List(ListData *data_1, ListData *data_2) {
    int i;
    for (i = 0; i < DIES_SETMANA; i++) {
        data_1->totalRetrasos[i] += data_2->totalRetrasos[i];
        data_1->numeroRetrasos[i] += data_2->numeroRetrasos[i];
    }
}

/**
 * Calcula la mitjana d'un dia d'un vol
 * Arguments:
 * 		RBTree *tree: árbol
 *              char* aeropuertoOrigen: Cadena del aeropuerto origen
 *              char *aeropuertoDestino: Cadena del aeropuerto destino
 *              int diaVuelo: Día para calcular la media
 * Retorna: 
 *      La media de un día de la semana dado el aeropuerto origen y aeropuerto destino
 */

float mitjana_diaria(RBTree *tree, char *aeroportOrigen, char *aeroportDesti, int diaVol) {

    RBData *nodoAeropuertoOrigen; /* Nodo con toda la información */
    ListData *linkAeropuertoDestino;
    float resultado;
    /* Buscamos el aeropuerto origen en el árbol*/
    nodoAeropuertoOrigen = findNode(tree, aeroportOrigen);


    if (nodoAeropuertoOrigen == NULL) {
        printf("El aeropuerto %s debe existir\n", aeroportOrigen);
        return 0;
    }

    /* Compruebo si ya se ha hecho algun viaje al aeropuerto destino. */
    linkAeropuertoDestino = findList(nodoAeropuertoOrigen->aeropuertosDestino, aeroportDesti);

    if (linkAeropuertoDestino == NULL) {
        printf("No se ha encontrado en el aeropuerto origen %s el aeropuerto %s\n",
               nodoAeropuertoOrigen->key, aeroportDesti);
        return 0;
    }
    /* Comprobamos si tiene algun retraso */
    if (linkAeropuertoDestino->numeroRetrasos[diaVol - 1] == 0) {
        printf("No hay ningún retraso\n");
        return 0;
    }

    resultado = (float) linkAeropuertoDestino->totalRetrasos[diaVol - 1] /
                (float) linkAeropuertoDestino->numeroRetrasos[diaVol - 1];
    return resultado;
}

/**
 * Función que añade un aeropuerto origen
 * Argumentos:
 * 		RBTree *tree: árbol
 *              struct InformacionVuelo* infoVuelo: Información de la línea leída
 */

void addInfoVuelo(RBTree *tree, ListData *data) {

    List *listaAeropuertosDestino; /* Lista donde se guardarán los aeropuertos destino */
    RBData *nodoAeropuertoOrigen; /* Nodo con toda la información */
    /* Inicializamos la lista dónde guardaremos los aeropuertos destino. */
    listaAeropuertosDestino = (List *) malloc(sizeof(List));
    initList(listaAeropuertosDestino);

    /* Si no está, reservamos memoria para añadirlo */
    nodoAeropuertoOrigen = (RBData *) malloc(sizeof(RBData));

    /* Ponemos como key, el aeropuerto origen*/
    nodoAeropuertoOrigen->key = (char *) malloc(sizeof(char) * 4);
    strcpy(nodoAeropuertoOrigen->key, data->key_sec);

    /* Añadimos la lista vacía que contendrá los aeropuertos destino*/
    nodoAeropuertoOrigen->aeropuertosDestino = listaAeropuertosDestino;
    /* Finalmente, añadimos los atrasos que haya en esa línea */
    addInfoRetraso(nodoAeropuertoOrigen, data);
    insertNode(tree, nodoAeropuertoOrigen);
}


void addInfoRetraso(RBData *treeData, ListData *data) {
    ListData *listData;
    /* Compruebo si ya se ha hecho algun viaje al aeropuerto destino.*/
    listData = findList(treeData->aeropuertosDestino, data->key);

    /* Buscamos el aeropuerto destino en el nodo aeropuerto origen
       Si no existe quiere decir que en aún no se ha registrado el aeropuerto destino en el nodo.*/
    if (listData == NULL) {
        /* Reservamos memoria*/
        listData = malloc(sizeof(ListData));
        listData->key = (char *) malloc(sizeof(char) * 4);
        listData->key_sec = (char *) malloc(sizeof(char) * 4);
        strcpy(listData->key, data->key);
        strcpy(listData->key_sec, data->key_sec);

        listData->totalRetrasos = (int *) calloc(DIES_SETMANA, sizeof(int));
        listData->numeroRetrasos = (int *) calloc(DIES_SETMANA, sizeof(int));

        /* Pasamos la informacion de las listas de la tabla hash a la lista del arbol*/
        transferDataHash2List(listData, data);

        insertList(treeData->aeropuertosDestino, listData);
    } else {
        transferDataHash2List(listData, data);
    }

}


/**
 * Función que añade un retraso
 * Argumentos:
 * 		List *aeropuertoOrigen
 *      struct InformacionVuelo* infoVuelo: Información de la línea leída
 */
void add2Hash(List *aeropuertoOrigen, struct InformacionVuelo *infoVuelo) {
    ListData *linkAeropuerto;

    linkAeropuerto = findList_2k(aeropuertoOrigen, infoVuelo->aeropuertoDestino, infoVuelo->aeropuertoOrigen);

    /* Buscamos el aeropuerto destino en el nodo aeropuerto origen
       Si no existe quiere decir que en aún no se ha registrado el aeropuerto destino en el nodo.*/
    if (linkAeropuerto == NULL) {

        /* Reservamos memoria*/
        linkAeropuerto = malloc(sizeof(ListData));
        linkAeropuerto->key = (char *) malloc(sizeof(char) * 4);
        linkAeropuerto->key_sec = (char *) malloc(sizeof(char) * 4);

        strcpy(linkAeropuerto->key, infoVuelo->aeropuertoDestino);


        /* Se añade la key del aeropuerto origen por especificación de la práctica*/
        strcpy(linkAeropuerto->key_sec, infoVuelo->aeropuertoOrigen);

        linkAeropuerto->totalRetrasos = (int *) calloc(DIES_SETMANA, sizeof(int));
        linkAeropuerto->numeroRetrasos = (int *) calloc(DIES_SETMANA, sizeof(int));

        /* Para ese dia de la semana concreto, se suman los retrasos acumulados con el actual del vuelo*/
        linkAeropuerto->totalRetrasos[(infoVuelo->diaVuelo) - 1] += infoVuelo->retrasoVuelo;
        linkAeropuerto->numeroRetrasos[(infoVuelo->diaVuelo) - 1] += 1;

        insertList(aeropuertoOrigen, linkAeropuerto);

    } else {
        linkAeropuerto->totalRetrasos[(infoVuelo->diaVuelo) - 1] += infoVuelo->retrasoVuelo;
        linkAeropuerto->numeroRetrasos[(infoVuelo->diaVuelo) - 1] += 1;
    }
}
