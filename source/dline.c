#include "../headers/constants.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"


doctors_list *doctors_list_head = NULL;

void insertDoctorToList(Doc *doctor) {
    doctors_list *newNode = (doctors_list*)calloc(1, sizeof(doctors_list));
    newNode->ldoctor = doctor;
    newNode->next = NULL;

    if (doctors_list_head == NULL) {
        doctors_list_head = newNode;
    } else {
        doctors_list *p = doctors_list_head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = newNode;
    }
}

Doc *getDoctorByName(const char *name) {
    doctors_list *p = doctors_list_head;
    while (p != NULL) {
        if (strcmp(p->ldoctor->Name, name) == 0) {
            return p->ldoctor;
        }
        p = p->next;
    }
    return NULL;
}

void printDoctorList() {
    doctors_list *p = doctors_list_head;
    int i = 1;
    while (p != NULL) {
        printf("%d. Name: %s Patients: %d\n", i, p->ldoctor->Name, p->ldoctor->nPatients);
        p = p->next;
        i++;
    }
}

void freeDoctorList() {
    doctors_list *p = doctors_list_head;
    while (p != NULL) {
        p = p->next;
        if (doctors_list_head->ldoctor->Name != NULL) {
            free(doctors_list_head->ldoctor->Name);
        }
        /*if (doctors_list_head->ldoctor->nLicense != NULL) {
            free(doctors_list_head->ldoctor->nLicense);
        }*/
        if (doctors_list_head->ldoctor != NULL) {
            free(doctors_list_head->ldoctor->Name);
            free(doctors_list_head->ldoctor->nLicense);
            free(doctors_list_head->ldoctor);
        }
        if (doctors_list_head != NULL) {
            free(doctors_list_head);
        }
        doctors_list_head = p;
    }
    doctors_list_head = NULL;
}

void saveDoctorList()
{
    FILE *doctors_file = NULL;
    doctors_list *pList = doctors_list_head;
    doctors_file = fopen(DOCTORS_TXT_FILE_PATH, "w");

    if (doctors_file == NULL)
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    fprintf(doctors_file , "Full Name; License Number; Number of Patients\n");
    fprintf(doctors_file, "=============================================\n");

    while (pList != NULL)
    {
        fprintf(doctors_file, "%s;%s;%d\n", pList->ldoctor->Name, pList->ldoctor->nLicense, pList->ldoctor->nPatients);
        pList = pList->next;
    }
    fclose(doctors_file);
}
