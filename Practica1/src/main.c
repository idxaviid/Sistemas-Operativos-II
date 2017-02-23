/**
 *
 * Main file 
 * 
 * This file calls the red-black-tree.c and linked-lit.c functions.
 *
 * Xavi Cano, September 2016.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define MAXCHAR  100
#define MAXLINES  10000

#include "red-black-tree.h"
#include "linked-list.h"




/** -------------------------------------------------- LLEGIR CSV ------------------------------------------------------- **/

void readCSV(char *filename)
{

  /** Declaracions de variables **/
  FILE *fp,*fp2;
  char *line,*col17,*col18;
  int i,int_col4,int_col15;
  

  /** Declaracions de memoria dinamica **/
  line = (char *) malloc(sizeof(char) * MAXCHAR);
  col17 = (char *) malloc(sizeof(char) * MAXLINES);
  col18 = (char *) malloc(sizeof(char) * MAXLINES);


  /** Obro el fitxer que conte les dades dels aeroports**/
  fp = fopen(filename, "r");
  if (!fp) {
    printf("Could not open file '%s'\n", filename);
    exit(1);
  }

  /** Creo un fitxer nou per guardar el codi IATA de cada aeroport d'origen**/
  fp2 = fopen("dades_node.csv", "w");
  if (!fp2) {
      printf("Could not create file");
      exit(1);
  }

  /** Bucle que itera per cada linea del fitxer **/
  while (fgets(line, MAXCHAR, fp) != NULL)
  {

    i = 1;

    /** Separo les cadenes per comes amb la funció strtok() així tinc a cada posicio del vector cad les paraules **/
    char *cad = strtok(line, ",");

    /** Bucle que recorre el vector cad **/
    while(cad)
    {
      /** Guardo el dia de la setmana del vol a col4 com a sencer mitjançant atoi() **/
      if(i==4){int_col4 = atoi(cad);fprintf(fp2,"%d,",int_col4);}
      /** Guardo el retard d'arribada a l'aeroport destí a col15 com a sencer mitjançant atoi() **/
      if(i==15){int_col15 = atoi(cad);fprintf(fp2,"%d,",int_col15);}
      /** Copio mitjançant strcpy() la cadena que conté l'aeroport d'origen a col17 **/
      if(i==17){strcpy(col17,cad);fprintf(fp2,"%s,",col17);}
      /** Copio mitjançant strcpy() la cadena que conté l'aeroport destí a col18 **/
      if(i==18){strcpy(col18,cad);fprintf(fp2,"%s\n",col18);}
      /** Paso a la seguent linea del fitxer**/
      i++;
      cad=strtok(NULL, ",");
    }
  }

  /** Tanco el fitxer **/
  fclose(fp);
  fclose(fp2);
  
  /** Allibero memoria dinamica **/
  free(line);
  free(col17);
  free(col18);
}




/** ------------------------------------------ MAIN -------------------------------------------------- **/

int main(int argc, char **argv)
{

  /** Comprovo que puc llegir el fitxer passat pel terminal **/
  if (argc != 2) {printf("Usage: %s <fitxer.csv>\n", argv[0]);exit(1);}

  /** Truco a la funcio que llegeix el csv i genera un nou csv amb les dades que m'intersen (dia,retard,origen,desti) **/
  readCSV(argv[1]);


  /** Declaracions de variables, punters i estructures **/
  FILE *fp3;
  char *line,*origen,*desti;
  int i;
  RBTree *tree;
  RBData *treeData;
  List *list;
  ListData *listData;

  /** Reservo memoria dinamica **/
  line = (char *) malloc(sizeof(char) * MAXCHAR);
  //desti = (char *) malloc(sizeof(char) * MAXLINES);

  /** Comprovo que puc llegir el nou fitxer csv **/
  fp3 = fopen("dades_node.csv", "r");
  if (!fp3) {printf("Could not open file %s\n","dades_node.csv");exit(1);}


  /** Reservo memeoria dinamica per l'arbre i la llista enllaçada **/
  tree = (RBTree *) malloc(sizeof(RBTree));
  list = (List *) malloc(sizeof(List));

  /** Inicio l'arbre i la llista **/
  initTree(tree);
  initList(list);

  /** Bucle que itera per cada linea del fitxer **/
  while (fgets(line, MAXCHAR, fp3) != NULL)
  {

    i = 1;

    /** Separo les cadenes per comes amb la funció strtok() així tinc a cada posicio del vector cad les paraules **/
    char *cad = strtok(line, ",");

    while(cad)
    {

      /** Informacio del dia i retard, aquesta info em falta com utilitzarla per això l'he comentat **/
      //if(i==1){dia = atoi(cad);}
      //if(i==2){retard = atoi(cad);}

      /** Si es la columna 3 vol dir que es un codi IATA d'origen **/
      if(i==3)
      {
        origen = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(origen,cad);
        treeData = findNode(tree, origen); 

        if (treeData != NULL) {

          /** Si la key esta en el arbre incremento 'num' */
          treeData->num++;
        } else {

          /** Si la key no hi es, reservo memoria dinamica per les dades i l'inserto a l'arbre **/
          treeData = malloc(sizeof(RBData));
          treeData->key = origen;
          treeData->num = 1;

          /** Comprovo que el codi sigui realment de 3 caracters tal com diu a la practica **/
          if (strlen(origen) == 3)
          {
            insertNode(tree, treeData);
          }
          
          printf("%s\n", origen);
        }

      }
      /** Si es la columna 4 vol dir que es un codi IATA destí **/
      if(i==4)
      {
        desti = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(desti,cad);
        listData = findList(list, desti);

        if (listData != NULL) {

          /** Incrementem el numero de vegades que ha aparegut un item **/
          listData->numTimes++;
        } else {

          /** Si la key no hi es a la llista, reservem memoria dinamica  per les dades i l'inserim a la llista **/
          listData = malloc(sizeof(ListData));
          listData->key = desti;
          listData->numTimes = 1;
          insertList(list, listData);
        }
      }
      /** Paso a la seguent linea del fitxer **/
      i++;
      cad=strtok(NULL, ",");
    }
  }

  printf("Numero d'aeroports d'origen insertats: %d\n",tree->numNodes);

  /** Alliberem memoria dinamica **/
  fclose(fp3);
  free(line);
  free(desti);
  deleteTree(tree);
  free(tree);

  printf("Printing contents of list:\n");

  /** Volquem els continguts a la sortida estandard **/
  dumpList(list);

  /** Alliberem memoria dinamica **/
  deleteList(list);
  free(list);

  printf("Done.\n");

  return 0;
}

