/**
 *
 * Main file 
 * 
 * Lluis Garrido, 2016.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#define MAXCHAR  100
#define MAXLINES  10000

/**
 *
 * This file reads and outputs the lines of a CSV file. 
 *
 */

void readCSV(char *filename)
{

  /** Declaracions de variables **/
  FILE *fp;
  char *line,*col4,*col15,*col17,*col18;
  int i,int_col4,int_col15;
  

  /** Declaracions de memoria dinamica **/
  line = (char *) malloc(sizeof(char) * MAXCHAR);
  col4 = (char *) malloc(sizeof(char) * MAXLINES);
  col15 = (char *) malloc(sizeof(char) * MAXLINES);
  col17 = (char *) malloc(sizeof(char) * MAXLINES);
  col18 = (char *) malloc(sizeof(char) * MAXLINES);


  /** Obro el fitxer **/
  fp = fopen(filename, "r");
  if (!fp) {
    printf("Could not open file '%s'\n", filename);
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
  		if(i==4){int_col4 = atoi(cad);}
      /** Guardo el retard d'arribada a l'aeroport destí a col15 com a sencer mitjançant atoi() **/
  		if(i==15){int_col15 = atoi(cad);}
  		/** Copio mitjançant strcpy() la cadena que conté l'aeroport d'origen a col17 **/
      if(i==17){strcpy(col17,cad);}
      /** Copio mitjançant strcpy() la cadena que conté l'aeroport destí a col18 **/
  		if(i==18){strcpy(col18,cad);}

  		i++;
  		cad=strtok(NULL, ",");
  	}
     printf("%d %d %s %s\n", int_col4,int_col15,col17,col18);
  }
  
  /** Tanco el fitxer **/
  fclose(fp);

  /** Allibero memoria dinamica **/
  free(line);
  free(col4);
  free(col15);
  free(col17);
  free(col18);
}

/**
 *
 * Main function. 
 *
 */

int main(int argc, char **argv)
{
  if (argc != 2) {
    printf("Usage: %s <fitxer.txt>\n", argv[0]);
    exit(1);
  }

  readCSV(argv[1]);

  return 0;
}

