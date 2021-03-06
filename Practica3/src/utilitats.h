#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "linked-list.h"
#include "vector.h"
#include "red-black-tree.h"

void llegirCamps(char * linea, List **hashTable);
void procesarLineas(Vector *vectorLineas,List **hashTable);
Vector* readCSV(FILE *fp, int *terminado);

void iterate(RBTree *tree, FILE *fp);

void saveTree(RBTree *tree, char *filename);
RBTree *loadTree(char *filename);
int drawGnuplot(RBTree *tree, char *origen, char *desti);

#endif /* UTIL_H*/

