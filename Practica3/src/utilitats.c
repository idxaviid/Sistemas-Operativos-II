/*
 *  Descripcion: Funciones utiles .
 */

#include "utilitats.h"
#include "taula_hash.h"
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

/**
 *  We need to include here "sentinel" since we use NIL in this file
 *
 */

extern Node sentinel;

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



/** 
 *
 *  Saves recursively the contents of each node. This function is called by
 *  saveTree. Do not call directly.
 *
 */

void saveTreeRecursive(Node *x, FILE *fp)
{
  int len;

  len = strlen(x->data->key);

  fwrite(&len, sizeof(int), 1, fp);
  fwrite(x->data->key, sizeof(char), len, fp);

  if (x->right != NIL)
    saveTreeRecursive(x->right, fp);

  if (x->left != NIL)
    saveTreeRecursive(x->left, fp);
}

/**
 *
 * Guarda l'arbre en un fitxer
 *
 */

void saveTree(RBTree *tree, char *filename)
{
  FILE *fp;
  
  fp = fopen(filename, "w");
  if (!fp) {
    printf("No he pogut obrir el fitxer.\n");
    return;
  }

  fwrite(&(tree->numNodes), sizeof(int), 1, fp);
  saveTreeRecursive(tree->root, fp);
  fclose(fp);
}


/**
 *
 * Llegix l'arbre de disc
 *
 */

RBTree *loadTree(char *filename)
{
  int i, len, numNodes;

  FILE *fp;

  RBTree *tree;
  RBData *data;

  fp = fopen(filename, "r");
  if (!fp) {
    printf("No he pogut obrir el fitxer.\n");
    return NULL;
  }

  tree = malloc(sizeof(RBTree));
  initTree(tree);
  fread(&numNodes, 1, sizeof(int), fp);
  printf("Nombre de nodes de l'arbre: %d\n", numNodes);


  for(i = 0; i < numNodes; i++)
  {
    data = malloc(sizeof(RBData));

    fread(&len, sizeof(int), 1, fp);
    data->key = malloc(sizeof(char) * (len + 1));

    fread(data->key, sizeof(char), len, fp);
    data->key[len] = 0;
    insertNode(tree, data);
  }

  fclose(fp);

  printf("Arbre llegit ;-)\n");
  return tree;
}


/**
 *
 * Dibuixa la grafica del retard mig entre dos aeroports qualsevol
 *
 */

int drawGnuplot(RBTree *tree, char *origen, char *desti)
{
    char *cercar;
    int dia,i;
    float mitjanaRetras;
    RBData *aeroportOrigen;
    RBData *aeroportDesti;
    cercar = origen;
    aeroportOrigen = findNode(tree, cercar);
    cercar = desti;
    aeroportDesti = findNode(tree, cercar);

    //Comprobo que els aeroports existeixen realment a l'arbre
    if (aeroportOrigen && aeroportDesti)
    {
        printAeroport(aeroportOrigen);
        char *filename = "dades.dat";
        FILE *fp;
        fp = fopen("dades.dat", "w");
        if (!fp) 
        {
            printf("Could not create pipe file %s \n",filename);
        }
        //Mostro la mitjana del retras de cada dia de la setmana
        for (i = 1; i <= 7; i++)
        {
            dia = i;
            mitjanaRetras = mitjana_diaria(tree, origen, desti, dia);
            printf("Retard mig dia: %d Aeroport origen %s, aeroport desti %s: %f minuts\n", dia,origen,desti, mitjanaRetras);
            fprintf(fp,"%d %f\n", dia, mitjanaRetras);
        }

        fclose(fp);
        char line[300];
        FILE *fpin, *fpout;
        fpin = fopen(filename, "r");
        
        if (!fpin) 
        {
        printf("ERROR: no puc obrir fitxer d'entrada.\n");
        return 0;
        }

        fpout = popen("gnuplot -persist", "w");

        if (!fpout)
        {
        printf("ERROR: no puc crear canonada.\n");
        return 0;
        }

        strcpy(line,"set term svg\n");
        fputs(line,fpout);
        strcpy(line,"set title 'GRAFICA DEL RETARD MIG ENTRE DOS AEROPORTS QUALSEVOL'\n");
        fputs(line,fpout);
        strcpy(line,"set style line 1 lc rgb '#0060ad' lt 1 lw 2 pt 7 ps 1.0\n");
        fputs(line,fpout);
        strcpy(line,"set grid xtics\n");
        fputs(line,fpout);
        strcpy(line,"set grid ytics\n");
        fputs(line,fpout);
        strcpy(line,"set xlabel 'Dies( 1 = Dilluns, 2 = Dimarts, etc )'\n");
        fputs(line,fpout);
        strcpy(line,"set ylabel 'Retras( minuts )'\n");
        fputs(line,fpout);
        strcpy(line,"set yrange[-100:100]\n");
        fputs(line,fpout);
        strcpy(line,"set out 'grafic.svg'\n");
        fputs(line,fpout);
        strcpy(line,"plot 'dades.dat' with linespoints ls 1\n");
        fputs(line,fpout);
        strcpy(line,"quit\n");
        fputs(line,fpout);

        if (fputs(line, fpout) == EOF) 
        {
            printf("ERROR: no puc escriure a la canonada.\n");
            return 0;
        }
        fflush(stdout);

        if (pclose(fpout) == -1)
        {
            printf("ERROR: pclose.\n");
            return 0;
        }
        if (pclose(fpin) == -1)
        {
            printf("ERROR: pclose.\n");
            return 0;
        }
        printf("L'aplicació ha acabat.\n");

        return 0;

    }else
    {
        printf("no esta en el arbol\n");
        return 0;
        
    }

}