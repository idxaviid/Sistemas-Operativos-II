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
#include <time.h>

#define MAXCHAR  100 
#define MAXLINES  7009728
#define N 10
#define HASH_SIZE  100

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

  /* Tanco el fitxer */
  fclose(fp);
  fclose(fp2);
  
  /* Allibero memoria dinamica */
  free(line);
  free(col17);
  free(col18);
}



/** -------------------------------------------------- LLEGIR N BLOCKS ------------------------------------------------------- **/

void read_N_blocks()
{

  /* Declaracions de tipus de variables */
  FILE *fp3,*fp4;
  char *line,*origen,*desti,*dia,*retard;
  int i,cnt_lines=0;

  /* Reservo memoria dinamica */
  line = (char *) malloc(sizeof(char) * MAXCHAR);

  /* Comprovo que puc llegir el nou fitxer csv */
  fp3 = fopen("dades_node.csv", "r");
  if (!fp3) {printf("Could not open file %s\n","dades_node.csv");exit(1);}


  fp4 = fopen("blocs_dades_node.csv", "w");
  if (!fp4) {printf("Could not open file %s\n","dades_node.csv");exit(1);}

  /* Bucle que itera per cada linea del fitxer fins que arriba a N*/
  while ((fgets(line, MAXCHAR, fp3) != NULL) && (cnt_lines != N))
  {
    i = 1;
    /* Separo les cadenes per comes amb la funció strtok() així tinc a cada posicio del vector cad les paraules */
    char *cad = strtok(line, ",");

    while(cad)
    {

      /* Si es la columna 1 vol dir que correspon al dia de la setmana */
      if(i==1)
      {
        dia = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(dia,cad);
        fprintf(fp4,"%s,",dia);
      }
      /* Si es la columna 2 vol dir que correspon al retard */
      if(i==2)
      {
        retard = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(retard,cad);
        fprintf(fp4,"%s,",retard);
      }

      /* Si es la columna 3 vol dir que es un codi IATA d'origen */
      if(i==3)
      {
        origen = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(origen,cad);
        fprintf(fp4,"%s,",origen);
      }
      /* Si es la columna 4 vol dir que es un codi IATA destí */
      if(i==4)
      {
        desti = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(desti,cad);
        fprintf(fp4,"%s",desti);
      }
      /* Paso a la seguent linea del fitxer */
      i++;
      cad=strtok(NULL, ",");
    }
    /* Contador per saber quina mida de bloc de N lines es vol llegir */
    cnt_lines++;
  }

  /* Alliberem memoria dinamica */
  fclose(fp3);
  fclose(fp4);
  free(line);
  free(origen);
  free(desti);
  free(dia);
  free(retard);
}

/** -------------------------------------------------- DONAM INDEX HASH ------------------------------------------------------- **/

int getHashValue(char *cadena)
{
  unsigned int i, len, seed, sum, hash;

  len = strlen(cadena); 
  sum = 0;
  seed = 131;
  for(i = 0; i < len; i++)
    sum = sum * seed + (int)cadena[i];

  hash = sum % HASH_SIZE;

  return hash;
}

/** -------------------------------------------------- CREA TAULA HASH ------------------------------------------------------- **/

List *allocHashTable()
{
  int i;
  List *hashTable;

  hashTable = malloc(sizeof(List) * HASH_SIZE);
  for(i = 0; i < HASH_SIZE; i++)
    initList(&(hashTable[i]));

  return hashTable;
}


/** -------------------------------------------------- ALLIBERA TAULA HASH ------------------------------------------------------- **/

void deleteHashTable(List *hashTable)
{
  int i;

  for(i = 0; i < HASH_SIZE; i++)
    deleteList(&(hashTable[i]));

  free(hashTable);
}


/** -------------------------------------------------- INSERCCIO DADES EN TAULA HASH I ARBRE ------------------------------------------------------- **/

void insert_data_table_hash_tree()
{
  /* Declaracions de tipus de variables */
  FILE *fp3;
  char *line,*origen,*desti,*desti_copy,*dia,*retard;
  int i,valor_hash,int_retard,cnt_lines=0;
  int mitja_retard=0;
  RBTree *tree;
  RBData *treeData;
  List *list;
  ListData *listData;
  List *hashTable;

  /* Reservo memoria dinamica */
  line = (char *) malloc(sizeof(char) * MAXCHAR);

  fp3 = fopen("blocs_dades_node.csv", "r");
  if (!fp3) {printf("Could not open file %s\n","blocs_dades_node.csv");exit(1);}

  /* Reservo memeoria dinamica per l'arbre i la llista enllaçada */
  tree = (RBTree *) malloc(sizeof(RBTree));
  list = (List *) malloc(sizeof(List));

  /* Inicio l'arbre i la llista */
  initTree(tree);
  initList(list);

  /* Hem guardo la taula Hash */
  hashTable = allocHashTable();

  /* Bucle que itera el nou ficher sencer de N blocks */ 
  while (fgets(line, MAXCHAR, fp3) != NULL)
  {
    i = 1;
    char *cad = strtok(line, ",");

    while(cad)
    {
      /* Si es la columna 1 vol dir que correspon al dia de la setmana */
      if(i==1)
      {
        dia = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(dia,cad);
      }
      /* Si es la columna 2 vol dir que correspon al retard */
      if(i==2)
      {
        retard = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(retard,cad);
        int_retard = atoi(retard);
        mitja_retard = (mitja_retard + (int)int_retard); 
      }
      /* Si es la columna 3 vol dir que es un codi IATA d'origen */
      if(i==3)
      {
        origen = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(origen,cad);
        treeData = findNode(tree, origen); 

        if (treeData != NULL) {
          treeData->num++;
        } else {

          treeData = malloc(sizeof(RBData));
          treeData->key = origen;
          treeData->num = 1;

          if (strlen(origen) == 3)
          {
            insertNode(tree, treeData);
          }
          printf("Aeroport d'origen: %s\n", origen);
        }
      }
      /* Si es la columna 4 vol dir que es un codi IATA destí */
      if(i==4)
      {
        desti = malloc(sizeof(char) * (strlen(cad) + 1));
        strcpy(desti,cad);

        valor_hash = getHashValue(origen);
        printf("Valor_Hash[%d]: %d\n",cnt_lines,valor_hash);
        listData = findList(&(hashTable[valor_hash]), origen);

        if (listData != NULL) {
          listData->numTimes++;
        } else {
          desti_copy = malloc(sizeof(char) * (strlen(cad)+1));
          strcpy(desti_copy, desti);
          listData = malloc(sizeof(ListData));
          listData->key = desti_copy;
          listData->key_sec = origen;
          listData->day = dia;
          listData->delay = retard;
          listData->numTimes = 1;
          insertList(&(hashTable[valor_hash]), listData);
          //printf("%s,%s,%s,%s", listData->day,listData->delay, listData->key,listData->key_sec);
        }
      }
      i++;
      cad=strtok(NULL, ",");
    }
    cnt_lines++;
  }

  ListItem *current;
  int k, l, numItems;

  for(k = 0; k < HASH_SIZE; k++) 
  {
    numItems = hashTable[k].numItems;
    current = hashTable[k].first;
    for(l = 0; l < numItems; l++) 
    {
      treeData = findNode(tree, current->data->key); 
      if (treeData != NULL) {
        treeData->num++;
      } else {
        treeData = malloc(sizeof(RBData));
        treeData->key = listData->key;
        treeData->num = 1;
        if (strlen(listData->key) == 3)
        {
          insertNode(tree, treeData);
        }
      }
      current = current->next;
    }
  }

  printf("\n\nNumero d'aeroports d'origen insertats: %d\n",tree->numNodes);
  printf("Mitja de retard dels aeroports: %d minuts\n",(mitja_retard/N));

  /** Alliberem memoria dinamica **/
  fclose(fp3);
  free(line);
  free(origen);
  free(desti);
  free(dia);
  free(retard);
  deleteTree(tree);
  free(tree);

  /* Volquem els continguts a la sortida estandard */
  dumpList(list);
  deleteHashTable(hashTable);
  /* Alliberem memoria dinamica */
  deleteList(list);
  free(list);
}


/** ----------------------------------------------------- MAIN -------------------------------------------------------------- **/

int main(int argc, char **argv)
{

  clock_t begin = clock();
  /* Comprovo que puc llegir el fitxer passat pel terminal */
  if (argc != 2) {printf("Usage: %s <fitxer.csv>\n", argv[0]);exit(1);}
  readCSV(argv[1]);
  read_N_blocks();
  insert_data_table_hash_tree();
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("Temps d'execució: %lf segons\n",time_spent);
  return 0;
}






