/*
 *  Descripcion: Funciones utiles .
 */

#include "utilitats.h"
#include "taula_hash.h"
#include "globals.h"

#include <ctype.h>

#define LONGITUD_MAXIMAVUELO 3

/**
 * Función que permite comprobar si el código de un aeropuerto supera la longitud permitida
 */


/**
 * Función que permite leer todos los campos de la línea de vuelo.
 * Argumentos:
 * 		char* linea: Indica la linea con toda la información de un vuelo
 */

void llegirCamps(char * linea, List **hashTable) {

    InformacionVuelo* infoVol;
    /*Recorre la linea por caracteres*/
    char caracter;
    /* i sirve para recorrer la linea
     * iterator es para copiar el substring de la linea a char
     * comaCount es el contador de comas
     */
    int i, iterator, comaCount;
    /* start indica donde empieza el substring a copiar
     * end indica donde termina el substring a copiar
     * len indica la longitud del substring
     */
    int start, end, len;
    /* valid nos permite saber si todos los campos son correctos
     * 1 hay error, 0 no hay error pero no hemos terminado, 2 no hay error y hemos terminado
     */
    int valid = 0;
    /*
     * eow es el caracter de fin de palabra
     */
    char eow = '\0';
    /*
     * contenedor del substring a copiar
     */
    char *word;
    /*
     * Reservamos memoria
     */
    infoVol = (InformacionVuelo*) malloc(sizeof (InformacionVuelo));
    infoVol->aeropuertoDestino = (char*)malloc(sizeof(char)*4);
    infoVol->aeropuertoOrigen = (char*)malloc(sizeof(char)*4);
    /*
     * Inicializamos los valores de las variables
     */
    start = 0;
    end = -1;
    i = 0;
    comaCount = 0;
    /*
     * Empezamos a contar comas
     */
    do {
        caracter = linea[i++];
        if (caracter == ',') {
            comaCount ++;
            /*
             * Cogemos el valor de end
             */
            end = i;
            /*
             * Si es uno de los campos que queremos procedemos a copiar el substring
             */
            if(comaCount == DIA_VUELO || comaCount == ATRASO_LLEGADA_AEROPUERTO || comaCount == AEROPUERTO_ORIGEN || comaCount == AEROPUERTO_DESTINO){
                /*
                 * Calculamos la longitud, si es mayor que 1 es que tenemos algo que copiar
                 */
                len = end - start;
                if (len > 1){
                    /*
                     * Alojamos memoria y copiamos el substring
                     */
                    word =(char*)malloc(sizeof(char)*(len));
                    for(iterator = start; iterator < end-1; iterator ++){
                        word[iterator-start] = linea[iterator];
                    }
                    /*
                     * Introducimos el caracter de fin de palabra
                     */
                    word[iterator-start] = eow;
                    /*
                     * Copiamos el contenido de word dentro del respectivo campo de infoVuelo
                     */
                    if(comaCount == DIA_VUELO){
                        infoVol->diaVuelo = atoi(word);
                    }else if(comaCount == ATRASO_LLEGADA_AEROPUERTO){
                        infoVol->retrasoVuelo = atoi(word);
                    }else if(comaCount == AEROPUERTO_ORIGEN || comaCount == AEROPUERTO_DESTINO){
                        /*
                         * Comprobamos que los codigos IATA tengan 3 letras
                         */
                        if(strlen(word)==3){
                            if(comaCount == AEROPUERTO_ORIGEN){
                                strcpy(infoVol->aeropuertoOrigen,word);
                            }else{
                                strcpy(infoVol->aeropuertoDestino,word);
                                valid = 2;
                            }
                        }else{
                            valid = 1;
                        }
                    }
                    free(word);
                }else{
                    valid = 1;
                }
            }
            start = end;
        }
    } while (caracter && valid==0);

    if (valid == 2) {
        insertarAeropuerto(hashTable, infoVol);
    }
    /*
     * Liberacion de recursos
     */
    free(infoVol->aeropuertoOrigen);
    free(infoVol->aeropuertoDestino);
    free(infoVol);
}

void procesarLineas(Vector *vectorLineas,List **hashTable) {
    int i;

    for (i = 0; i < vectorLineas->numElementos; i++) {
        llegirCamps(vectorLineas->array[i], hashTable);
    }
    freeVector(vectorLineas);
}

/**
 *
 * This file reads and outputs the lines of a CSV file.
 *
 */
Vector* readCSV(FILE *fp, int *terminado) {
    char *line;
    long lineaLeidaBloque;
    Vector *vector;

    line = (char *) malloc(sizeof (char) * MAXCHAR);

    /* Leemos la primera linea si tiene una letra*/
    if (isdigit(fgets(line, MAXCHAR, fp)[0]) == 0) {
        fgets(line, MAXCHAR, fp);
    }

    /* Creamos una lista de Vectores. Cada vector será procesado en la proxima práctica por un hilo*/
    vector = (Vector *) malloc(sizeof(Vector));
    inicializarVector(vector);

    lineaLeidaBloque = 0;
    while (fgets(line, MAXCHAR, fp) != NULL && lineaLeidaBloque < NBlock){
        addElemento(vector, line);
        lineaLeidaBloque++;
    }
    if (lineaLeidaBloque < NBlock){
        *terminado = 1;
    }
    free(line);
    return vector;
}

void iterate(RBTree *tree, FILE *fp) {
    int terminado = 0;
    while( terminado != 1){
        List **hashTable;
        Vector* vector;
        vector = readCSV(fp, &terminado);
        hashTable = crearHashTable();
        procesarLineas(vector,hashTable);
        hashTable2RBTree(hashTable, tree);
    }
}
