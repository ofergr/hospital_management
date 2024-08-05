#ifndef PLINE_H
#define PLINE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Patient Patient;

typedef struct pInLine {
    Patient *lpatient;
    struct pInLine *next;
} pInLine;

extern pInLine *patients_line;

void insertPatientToLine(pInLine **head, Patient *patient);

void displayPatients();

void freePatientsQueue();

void clearAllPatientsInLine(pInLine** );

void removePatientFromLine(pInLine**, const char* );

#endif
