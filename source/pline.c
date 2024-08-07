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

/*
function to insert a new patient to the end of the line  The node holds reference to the patiet data which
is stored in the tree
*/
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

/* display the line of patients */
void displayPatients() {
    pInLine *p = patients_line;
    int i = 1;
    printf ("Patients in line\n");
    printf ("================\n");
    while (p != NULL) {
        printf("%d. Name: %s, ID: %s\n", i, p->lpatient->Name, p->lpatient->ID);
        p = p->next;
        i++;
    }
    printf("\n");
}

/* display the line of patients with more data*/
void displayPatientsInLine()
{
    pInLine *p = patients_line;
    Visit *visit = NULL;
    int i = 1;
    while (p != NULL)
    {
        printf("%d. Name: %s, ID: %s\n", i, p->lpatient->Name, p->lpatient->ID);
        visit = peek(p->lpatient->visits);
        printf("  Addmitted on %d/%d/%d %02d:%02d\n", visit->tArrival.Day, visit->tArrival.Month,
               visit->tArrival.Year, visit->tArrival.Hour,
               visit->tArrival.Min);
        p = p->next;
        i++;
    }
    printf("\n");
}

/* free patient line. Patient details are not freed, as they are only reference here. */
void freePatientsLine() {
    pInLine *p = patients_line;
    while (p != NULL) {
        p = p->next;
        if (patients_line->lpatient)
        {
            patients_line->lpatient = NULL;
        }
        free(patients_line);
        patients_line = p;

    }
    patients_line = NULL;
}

/* removes patient from line based on the patient's ID number. Whenfound, connect its previous node to its next */
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
    free(temp);
}

/* clear line of patients */
void clearAllPatientsInLine(pInLine** head) {
    pInLine* current = *head;
    pInLine* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    *head = NULL;
}

/* save patient's line to file */
void savePatientLine()
{
    FILE *patients_line_file = NULL;
    pInLine *p = patients_line;
    int cntr = 1;
    patients_line_file = fopen(PATIENTS_LINE_TXT_FILE_PATH, "w");
    if (patients_line_file == NULL)
    {
        perror("Error opening patient file");
        exit(EXIT_FAILURE);
    }
    fprintf(patients_line_file, "Patients' IDs in line\n");
    fprintf(patients_line_file, "=====================\n");
    while (p != NULL) {
        fprintf(patients_line_file, "%d.%s\n", cntr, p->lpatient->ID);
        p = p->next;
        cntr++;
    }
    fclose(patients_line_file);
}