
#include "taula_hash.h"
#include "globals.h"

/**
 * Función que calcula el valor hash
 * @param aeropuertoOrigen
 * @return 
 */
int getHashValue(char *aeropuertoOrigen) {
    unsigned int i, len, seed, sum, hash;
    char cadena[MAXCHAR];

    strcpy(cadena, aeropuertoOrigen);

    len = strlen(cadena);
    sum = 0;
    seed = 131;
    for (i = 0; i < len; i++) {
        sum = sum * seed + (int) cadena[i];
    }
    hash = sum % M;
    return hash;
}

/**
 * Función que crea una tabla hash con un tamaño dado
 * @param numElementos
 * @return  Retorna la tabla hash
 */
List **crearHashTable() {
    List **table = malloc(sizeof(List *) * M);
    int i;
    for (i = 0; i < M; i++) {
        table[i] = malloc(sizeof(ListData));
        initList(table[i]);
    }
    return table;
}


/*Obtiene el valor hash asociado al aeropuerto origen,
 *obtiene la lista de todos los aeropuertos destino asociados a la hash del aeropuerto origen
 *y Añade al Hash.
 */
void insertarAeropuerto(List **tablaHash, struct InformacionVuelo *infoVuelo) {
    int valorHash;
    List *list;

    valorHash = getHashValue(infoVuelo->aeropuertoOrigen);

    list = tablaHash[valorHash];

    add2Hash(list, infoVuelo);
}

/*Elimina una tabla y libera memoria.
 */
void deleteTable(List **table) {
    int i;
    for (i = 0; i < M; i++) {
        deleteList(table[i]);
        free(table[i]);
    }
}

/*Añade una hashTable a un RedBlack-tree
 * si existe añade infrmación de retraso, si no, añade información del vuelo
 */

void hashTable2RBTree(List **hashTable, RBTree *tree) {
    RBData *treeData;
    ListItem *current;
    ListData *data;
    int i;
    for (i = 0; i < M; i++) {
        current = hashTable[i]->first;
        while (current != NULL) {
            data = current->data;
            treeData = findNode(tree, data->key_sec);
            if (treeData != NULL) {
                addInfoRetraso(treeData, data);
            } else {
                addInfoVuelo(tree, data);
            }
            current = current->next;
        }
    }
    deleteTable(hashTable);
    free(hashTable);
}


