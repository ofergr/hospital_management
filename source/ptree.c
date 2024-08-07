#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"


pInTree *patients_tree = NULL;

/* allocation  of patient node */
pInTree* createTreeNode(Patient *patient) {
    pInTree *newNode = (pInTree*)calloc(1, sizeof(pInTree));
    if (newNode == NULL) {
        printf("Memory allocation failed for new tree node\n");
        exit(EXIT_FAILURE);
    }
    newNode->Patient = patient;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* insert patient to tree, in a recursive way */
pInTree *insertPatientToTree(pInTree *root, Patient *patient) {
    if (root == NULL) {
        return createTreeNode(patient);
    }

    if (strcmp(patient->ID, root->Patient->ID) < 0) {
        root->left = insertPatientToTree(root->left, patient);
    } else if (strcmp(patient->ID, root->Patient->ID) > 0) {
        root->right = insertPatientToTree(root->right, patient);
    } else {
        fprintf(stderr, "Duplicate ID found: %s\n", patient->ID);
    }

    return root;
}

/* search for patienr in the binary tress based on ID, reccursivly */
Patient *searchPatient(pInTree *patients_tree, char *id) {
    if (patients_tree == NULL) {
        return NULL;
    } else if (strcmp(patients_tree->Patient->ID, id) == 0) {
        return patients_tree->Patient;
    } else if (strcmp(id, patients_tree->Patient->ID) < 0) {
        return searchPatient(patients_tree->left, id);
    } else if (strcmp(id, patients_tree->Patient->ID) > 0) {
        return searchPatient(patients_tree->right, id);
    }
    return NULL;
}

/* display all patiens in the tree, recursivly */
void displayAllPatients(pInTree *patients_tree) {
    if (patients_tree != NULL) {
        displayAllPatients(patients_tree->left);
        printf("Name: %s, ID: %s\n", patients_tree->Patient->Name, patients_tree->Patient->ID);
        displayAllPatients(patients_tree->right);
    }
}

/* free all patients from the tree, recursivly */
void freeAllPatients(pInTree *patients_tree) {
    if (patients_tree == NULL) {
        return;
    }
    freeAllPatients(patients_tree->left);
    freeAllPatients(patients_tree->right);

    free(patients_tree);
    patients_tree = NULL;
}