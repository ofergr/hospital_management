#ifndef PATIENT_H
#define PATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pInTree pInTree;
typedef struct pInLine pInLine;

typedef struct Patient {
    char *Name;
    char *ID;
    char Allergies;
    StackVisits *visits;
    unsigned int nVisits;
} Patient;

Patient* createPatient(const char *name, const char *id, char allergies, unsigned int nVisits);

void admitNewPatient(pInTree *patients_tree);

pInLine *advancePatientInLine(pInLine *head, char *patient_id);

void dischargePatient();

void DisplayPatientAddmittions();

void removePatientFromTree(pInTree**, const char*);

void clearAllPatients(pInTree* );

#endif
