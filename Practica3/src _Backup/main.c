
#include <stdio.h>
#include <stdlib.h>
#include "red-black-tree.h"
#include "aeroport.h"
#include "utilitats.h"

/**
 *
 * Main function.
 *
 */

int main(int argc, char **argv) {
    FILE *fp;

    if (argc != 2) {
        printf("Use: %s <filename.csv>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Could not open file '%s'\n", argv[1]);
        exit(EXIT_FAILURE);
    }


    RBTree *tree;

    /* Reservem memoria*/
    tree = (RBTree *) malloc(sizeof(RBTree));

    /* Inicialitzem arbre */
    initTree(tree);
    iterate(tree, fp);
    fclose(fp);



    /* Prova per mostrar info d'aeroport DEN */
    char *cercar;
    int dia;
    float mitjanaDillunsDEN;
    RBData *aeroportOrigen;
    cercar = "DEN";
    aeroportOrigen = findNode(tree, cercar);
    printAeroport(aeroportOrigen);

    /*1= Dilluns*/
    dia = 7;

    mitjanaDillunsDEN = mitjana_diaria(tree, "DEN", "TPA", dia);
    printf("Mitjana dia: %i Aeroport origen DEN, aeroport desti TPA: %f minuts\n", dia, mitjanaDillunsDEN);

    /* Alliberem memoria */
    deleteTree(tree);
    free(tree);

    return EXIT_SUCCESS;
}

