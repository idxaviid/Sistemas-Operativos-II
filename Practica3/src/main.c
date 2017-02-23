
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include "red-black-tree.h"
#include "aeroport.h"
#include "utilitats.h"
#include "globals.h"





/*------------------------------------- MENU ------------------------------------------*/

int menu() 
{
  char str[5];
  int opcio;
  printf("------------------------------------------------------------\n");
  printf("|                         MENU                             |\n");
  printf("| 1 - Creacio de l'arbre                                   |\n");
  printf("| 2 - Emmagatzemar arbre a disc                            |\n");
  printf("| 3 - Llegir arbre de disc                                 |\n");
  printf("| 4 - Grafica del retard mig entre dos aeroports qualsevol |\n");
  printf("| 5 - Sortir                                               |\n");
  printf("------------------------------------------------------------\n\n");
  printf("Escull opcio: ");
  fgets(str, 5, stdin);
  opcio = atoi(str); 
  return opcio;
}


/*-------------------------------------- MAIN ----------------------------------------*/

int main() {
    
    /* Declaracions de variables */
    char filename[MAXCHAR],filename2[MAXCHAR];
    int opcio;
    RBTree *tree;
    tree = NULL;

    do {
        opcio = menu();
        printf("\n\n");

        switch (opcio) 
        {
            /* Creacio de l'arbre */
            case 1:
                //Comprobo si hi ha un arbre a memoria
                if (tree)
                {
                  printf("Alliberant arbre actual\n");
                  deleteTree(tree);
                  free(tree);
                }
                FILE *fp;
                printf("Introdueix fitxer que conte la base de dades: ");
                fgets(filename, MAXCHAR, stdin);
                filename[strlen(filename)-1]=0;
                fp = fopen(filename, "r");
                //Comprobo si puc obrir el fitxer
                if (!fp) 
                {
                  printf("No s'ha pogut obrir el fitxer '%s'\n", filename);
                  break;
                }
                tree = (RBTree *) malloc(sizeof(RBTree));
                initTree(tree);
                iterate(tree, fp);
                fclose(fp);
                printf("Arbre creat ;-)\n");
                break;

            /* Emmagatzemament de l’arbre */
            case 2:
                //Comprobo si hi ha un arbre a memoria
                if (!tree) 
                {
                  printf("No hi ha cap arbre a memoria\n");
                  break;
                }
                printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
                fgets(filename, MAXCHAR, stdin);
                filename[strlen(filename)-1]=0;
                printf("Arbre guardat ;-)\n");
                saveTree(tree, filename);
                break;

            /* Lectura de l’arbre */
            case 3:
                //Comprobo si hi ha un arbre a memoria
                if (tree) 
                {
                  printf("Alliberant arbre actual\n");
                  deleteTree(tree);
                  free(tree);
                }
                printf("Introdueix nom del fitxer amb l'arbre: ");
                fgets(filename, MAXCHAR, stdin);
                filename[strlen(filename)-1]=0;
                tree = loadTree(filename);
                break;

            /* Gràfica del retard mig */
            case 4:
                //Comprobo si hi ha un arbre a memoria
                if(!tree)
                {
                    printf("No hi ha cap arbre a memoria\n");
                    break;
                }
                printf("Introdueix l'aeroport d'origen: ");
                fgets(filename, MAXCHAR, stdin);
                filename[strlen(filename)-1]=0;
                printf("Introdueix l'aeroport destí: ");
                fgets(filename2, MAXCHAR, stdin);
                filename2[strlen(filename2)-1]=0;
                drawGnuplot(tree,filename,filename2);
                printf("Grafica creada ;-)\n");
                break;

            /* Sortida. */
            case 5:
                //Comprobo si hi ha un arbre a memoria
                if (tree) 
                {
                  exit(1);
                }
                break;

            default:
                printf("Opcio no valida\n");
        }
    }
    while (opcio != 5);
    return 0;
}

