#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"

pInLine *patients_line = NULL;

void insertPatientToLine(pInLine **head, Patient *patient) {
    pInLine *newNode = (pInLine*)calloc(1, sizeof(pInLine));
    newNode->lpatient = patient;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        pInLine *p = *head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = newNode;
    }
}

void displayPatients() {
    pInLine *p = patients_line;
    int i = 1;
    while (p != NULL) {
        printf("%d. Name: %s, ID: %s\n", i, p->lpatient->Name, p->lpatient->ID);
        p = p->next;
        i++;
    }
    printf("\n");
}

void freePatientsQueue() {
    pInLine *p = patients_line;
    while (p != NULL) {
        p = p->next;
        if (patients_line->lpatient)
        {
            free(patients_line->lpatient->Name);
            patients_line->lpatient->Name = NULL;
            free(patients_line->lpatient->ID);
            patients_line->lpatient->ID = NULL;
            free(patients_line->lpatient);
            patients_line->lpatient = NULL;
        }
        free(patients_line);
        patients_line = p;

    }
    patients_line = NULL;
}

void removePatientFromLine(pInLine** head, const char* patient_id) {
    if (*head == NULL) return;
    pInLine* temp = *head;
    pInLine* prev = NULL;
    while (temp != NULL && strcmp(temp->lpatient->ID, patient_id) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return; // Patient not found

    if (prev == NULL) {
        *head = temp->next;
    }
    else {
        prev->next = temp->next;
    }
    free(temp->lpatient->Name);
    free(temp->lpatient->ID);
    free(temp->lpatient);
    free(temp);
}

void clearAllPatientsInLine(pInLine** head) {
    pInLine* current = *head;
    pInLine* next;
    while (current != NULL) {
        next = current->next;
        /*free(current->lpatient->Name);
        free(current->lpatient->ID);
        free(current->lpatient);*/
        free(current);
        current = next;
    }
    *head = NULL;
}
