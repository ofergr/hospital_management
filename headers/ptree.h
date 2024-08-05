#ifndef PTREE_H
#define PTREE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Patient Patient;

typedef struct pInTree {
    Patient* Patient;
    struct pInTree *left;
    struct pInTree *right;
} pInTree;

extern pInTree *patients_tree;

pInTree* insertPatientToTree(pInTree *root, Patient *patient);

Patient *searchPatient(pInTree *patients_tree, char *id);

void displayAllPatients(pInTree *patients_tree);

void freeAllPatients(pInTree *patients_tree);

#endif
