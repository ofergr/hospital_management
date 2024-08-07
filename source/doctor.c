#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"

/* function to create a doctor object */
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
    newDoctor->nLicense = (char*)calloc(strlen(license) + 1, sizeof(char));
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

/* FUnction to display the doctor's list of patients, with deatils */
void DispalyDoctorPatients(pInLine *patients_line, char *docName)
{
    Patient *patient = NULL;
    pInLine *line = patients_line;
    Visit* visit = NULL;
    int cntr = 1;
    printf ("Doctor %s patients list:\n", docName);
    printf("====================================\n");
    while (line != NULL) {
        /* If a patient is currently being treated, it will be the visit on the top of his/her visits stack */
        visit = peek(line->lpatient->visits);
        if (_stricmp(visit->Doctor->Name, docName) == 0) {
            printf("%d. Name: %s, ID: %s, Addmittion date %d/%d/%d %02d:%02d\n", cntr, line->lpatient->Name, line->lpatient->ID,
                   visit->tArrival.Day, visit->tArrival.Month,
                   visit->tArrival.Year, visit->tArrival.Hour,
                    visit->tArrival.Min);
            cntr++;
        }
        line = line->next;
    }
    printf("\n");
}

/*
Release a patient from a doctor's care. In case we remove a patient and it has an active visit,
we need to decrease its doctor's patient count
*/
void releasePatientDoctor(char *patient_id)
{
  Patient *patient = searchPatient(patients_tree, patient_id);
  if (patient != NULL) {
    Visit *visit = peek(patient->visits);
    if (visit != NULL) {
        if (visit->tDismissed.Year == -1) {
            Doc *doctor = visit->Doctor;
            doctor->nPatients--;
        }
    }
  }
}