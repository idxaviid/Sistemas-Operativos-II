/**
 *
 * Linked-list header 
 * 
 * Include this file in order to be able to call the 
 * functions available in linked-list.c. We include
 * here only those information we want to make visible
 * to other files.
 *
 * Lluis Garrido, 2016.
 *
 */

/**
 * 
 * The TYPE_LIST_KEY is used to define the type of the primary
 * key used to index data in the list. 
 *
 */



/* Hemos añadido está instrucción para sólamente cargar la librería una vez y así evitarnos múltiples redefiniciones*/
#pragma once

/* La clave primaria utilizada es de tipo char*.
 * Con esta clave podremos mantener los códigos de los aeropuertos destino */
#define TYPE_LIST_KEY char *

/**
 *
 * This structure holds the information to be stored at each list item.  Change
 * this structure according to your needs.  In order to make this library work,
 * you also need to adapt the functions compEQ and freeListData. For the
 * current implementation the "key" member is used search within the list. 
 *
 */

typedef struct ListData_ {
    /* The variable used to index the list has to be called "key". */
    TYPE_LIST_KEY key; /* Aeropuerto destino */
    TYPE_LIST_KEY key_sec; /*Aeropuerto origen */

    int *totalRetrasos; /* Variable que contiene el total de retrasos de un dia de la semana*/
    int *numeroRetrasos; /*Variable que contiene el numero de retrasos guardados en un dia de la semana*/

    /* This is the additional information that will be stored
       within the structure. This additional information is associated
       to the key. You may include any field you may need useful.*/
    int numTimes;
} ListData;

/**
 * 
 * The item structure
 *
 */

typedef struct ListItem_ {
    ListData *data;
    struct ListItem_ *next;
} ListItem;

/**
 * 
 * The list structure
 *
 */

typedef struct List_ {
    int numItems;
    ListItem *first;
} List;

/**
 *
 * Function heders we want to make visible so that they
 * can be called from any other file.
 *
 */

void initList(List *l);
void insertList(List *l, ListData *data);
ListData *findList(List *l, TYPE_LIST_KEY key);
ListData *findList_2k(List *l, TYPE_LIST_KEY key, TYPE_LIST_KEY key_sec);
void deleteFirstList(List *l);
void deleteList(List *l);
void dumpList(List *l);

