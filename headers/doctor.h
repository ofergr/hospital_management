#ifndef DOCTOR_H
#define DOCTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENT_PER_DOC 4

typedef struct Doc {
    char *Name;
    char *nLicense;
    unsigned int nPatients;
} Doc;

Doc *createDoctor(const char *name, const char *license, const unsigned int patients);
void DispalyDoctorPatients(pInLine *patients_line, char *docName);
void decreasePatientCount(const char*);
#endif
