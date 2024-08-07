#include "../headers/constants.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"


doctors_list *doctors_list_head = NULL;

/*
Insert a Doctor into the line of doctors, at the end of the linked-list of doctors. Node has
references to Doctor objects, they do not hold the inforamtion itself.
*/
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

/* Look for a doctor in the list based on its name */
Doc *getDoctorByName(const char *name) {
    doctors_list *p = doctors_list_head;
    while (p != NULL) {
        if (_stricmp(p->ldoctor->Name, name) == 0) {
            return p->ldoctor;
        }
        p = p->next;
    }
    return NULL;
}

/* function to print all doctors in the list */
void printDoctorList() {
    doctors_list *p = doctors_list_head;
    int i = 1;

    printf("Doctors List\n");
    printf("============\n");

    while (p != NULL) {
        printf("%d. Name: %s Patients: %d\n", i, p->ldoctor->Name, p->ldoctor->nPatients);
        p = p->next;
        i++;
    }
    printf("\n");
}

/* function to free all the doctros from the list. */
void freeDoctorList() {
    doctors_list *p = doctors_list_head;
    while (p != NULL) {
        p = p->next;
        if (doctors_list_head->ldoctor->Name != NULL) {
            free(doctors_list_head->ldoctor->Name);
        }

        if (doctors_list_head->ldoctor != NULL) {;
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

/* Function to save the list of doctors and their details into the doctor file */
void saveDoctorList()
{
    FILE *doctors_file = NULL;
    doctors_list *pList = doctors_list_head;
    doctors_file = fopen(DOCTORS_TXT_FILE_PATH, "w");

    if (doctors_file == NULL)
    {
        perror("Error opening doctors file");
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
