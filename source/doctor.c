#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"

Doc* createDoctor(const char* name, const char* license, const unsigned int patients) {
    Doc* newDoctor = (Doc*)calloc(1, sizeof(Doc));
    if (newDoctor == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    newDoctor->Name = (char*)calloc(strlen(name) + 1, sizeof(char));
    if (newDoctor->Name == NULL) {
        // Handle memory allocation failure
        free(newDoctor);
        return NULL;
    }
    strcpy(newDoctor->Name, name);
    newDoctor->nLicense = (char*)calloc(strlen(license), sizeof(char));
    if (newDoctor->nLicense == NULL) {
        // Handle memory allocation failure
        free(newDoctor->Name);
        free(newDoctor);
        return NULL;
    }
    strcpy(newDoctor->nLicense, license);
    newDoctor->nPatients = patients;
    return newDoctor;
}

void DispalyDoctorPatients(pInLine *patients_line, char *docName)
{
    Patient *patient = NULL;
    pInLine *line = patients_line;
    Visit* visit = NULL;
    int cntr = 1;
    printf ("Doctor %s patients list:\n", docName);
    printf("====================================\n");
    while (line != NULL) {
        visit = peek(line->lpatient->visits);
        if (_stricmp(visit->Doctor->Name, docName) == 0) {
            printf("%d. Name: %s, ID: %s, Addmittiom date %d/%d/%d %d:%d\n", cntr, line->lpatient->Name, line->lpatient->ID,
                   visit->tArrival.Day, visit->tArrival.Month,
                   visit->tArrival.Year, visit->tArrival.Hour,
                    visit->tArrival.Min);
            cntr++;
        }
        line = line->next;
    }
    printf("\n");
}

