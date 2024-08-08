#include <stdio.h>

#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"

/* creates a patient object with patient's details */
Patient* createPatient(const char *name, const char *id, char allergies, unsigned int nVisits) {
    Patient* newPatient = (Patient*)calloc(1, sizeof(Patient));
    if (newPatient == NULL) {
        // handling memory allocation failar
        return NULL;
    }
    newPatient->Name = (char*)calloc(strlen(name) + 1, sizeof(char));
    if (newPatient->Name == NULL) {
        // handling memory allocation failar
        free(newPatient);
        return NULL;
    }
    strcpy(newPatient->Name, name);

    newPatient->ID = (char*)calloc(strlen(id) + 1, sizeof(char));
    if (newPatient->ID == NULL) {
        // handling memory allocation failar
        free(newPatient->Name);
        free(newPatient);
        return NULL;
    }
    strcpy(newPatient->ID, id);

    newPatient->Allergies = allergies;
    newPatient->nVisits = nVisits;
    newPatient->visits = (StackVisits *)calloc(1, sizeof(StackVisits));
    return newPatient;
}

/*
adminting new patient:
Get ID from user
Check if patient is already in the DB
If yes, add a new visit to the patient's visits stack
If not, get patient's details and create a new patient object. Add it to the DB (binary tree)
Assign a doctor to the patient
Create a new visit and push it to the patient's visits stack
Add the patient to the end of the patient's line
*/
void admitNewPatient(pInTree *patients_tree) {
    char patient_id[MAX_LINE_LENGTH] = { 0 }, patient_name[MAX_LINE_LENGTH] = { 0 };
    char allergies = 0;
    Doc* doctor = NULL;
    Patient *patient = NULL;
    Visit *visit = NULL;
    Date newDate;
    Date dismissed = createDate(-1, -1, -1, -1, -1);

    getUserId(patient_id);

    // see if patient is already in the DB
    patient = searchPatient(patients_tree, patient_id);
    if (patient == NULL) {
        // get patient details
        printf("Please enter your name: \n");
        fgets(patient_name, MAX_LINE_LENGTH, stdin);
        patient_name[strlen(patient_name) - 1] = '\0';
        allergies = getAllergiesFromUser();

        // after collecting patient details, create it and add it to the DB
        patient = createPatient(patient_name, patient_id, allergies, 1);
        patients_tree = insertPatientToTree(patients_tree, patient);
    }
    else {
        // Check if patient is already hospitalized
        Visit* lastVisit = peek(patient->visits);
        Date* date = &lastVisit->tDismissed;
        if (date->Year == -1) {
            printf("Patiant %s is already hospitolaized\n", patient_id);
            return;
        }
    }

    // get mew visit date
    getDateFromUser(&newDate);
    // assign a doctor
    doctor = assignDoctor2Case();
    // create a new visit
    visit = createVisit(newDate, dismissed, -1, doctor, NULL);
    // push the visit to the patient's visits stack
    push(patient->visits, visit);
    // push the patient to the end of the line
    insertPatientToLine(&patients_line, patient);
}

/* function to move a patient up the line */
pInLine *advancePatientInLine(pInLine *head, char *patient_id) {
    pInLine *curr = head;
    pInLine *prev = head;
    while (curr != NULL && strcmp(curr->lpatient->ID, patient_id) != 0) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == head)
    {
        printf("Patient id %s is already at the head of the patients list\n", patient_id);
        return curr;
    }
    if (curr == NULL) {
        printf("%s\n", ERROR_PATIENT_NOT_FOUND);
        return head;
    } else {
        prev->next = curr->next;
        curr->next = head;
    }
    return curr;
}

/*
Discharge a patient:
Get the patient's ID
Get discharge date from user
calculate the duration of the visit
Get a visit summary from the user
update the visit's details
*/
void dischargePatient()
{
    pInLine *p = patients_line;
    pInLine *prev = patients_line;
    char patient_id[MAX_LINE_LENGTH] = { 0 };
    char summary[MAX_LINE_LENGTH] = { 0 };
    Date destarchedDate;
    Patient *patient = NULL;
    Visit *lastVisit = NULL;
    float duration = 0;
    int dismissedDateValid = 0;

    getUserId(patient_id);

    // check if the patient is in line, if so, remove him from the it
    while (p != NULL) {
        if (strcmp(p->lpatient->ID, patient_id) == 0) {
            if (patients_line == p) {
                patients_line = p->next;
                free(p);
                break;
            }
            prev->next = p->next;
            free(p);
            break;
        }
        prev = p;
        p = p->next;
    }

    // look for the partient in the tree, and update discharge date and duration of visit
    patient = searchPatient(patients_tree, patient_id);
    if (patient != NULL) {
        Visit *lastVisit = peek(patient->visits);
        // get discharge date
        while (!dismissedDateValid)
        {
            printf("Please enter the discharge date: \n");
            getDateFromUser(&destarchedDate);
            duration = calculateDateDiff(&lastVisit->tArrival, &destarchedDate);
            if (duration < 0)
            {
                Date *arravial = &lastVisit->tArrival;
                printf("Dismissed date should be after arrival date (%02d/%02d/%02d %02d:%02d)\n",
                       arravial->Day, arravial->Month, arravial->Year,
                       arravial->Hour, arravial->Min);
            }
            else
            {
                dismissedDateValid = 1;
            }
        }

        lastVisit->tDismissed = destarchedDate;
        lastVisit->Duration = duration;
        printf("Please write a visit summary: \n");
        fgets(summary, MAX_LINE_LENGTH, stdin);
        summary[strlen(summary) - 1] = '\0';
        lastVisit->vSummary = calloc(1, MAX_LINE_LENGTH);
        if (lastVisit->vSummary != NULL)
        {
            strcpy(lastVisit->vSummary, summary);
        }
        lastVisit->Doctor->nPatients--;
        printf("Patient %s has been destarched. Duration of visit %d:%d hours\n\n", patient->Name,
               (int)(lastVisit->Duration) / MINUTES_IN_HOUR, (int)(lastVisit->Duration) % MINUTES_IN_HOUR);
    }
    else
        printf("%s\n", ERROR_PATIENT_NOT_FOUND);
}

/*
Display the list of a patient's visits, from latest to earliest.
As the visit stack is a LIFO, we will use an auxilary stack to reverse the order of the visits.
We will them print the list of visits, and push back the visits from the auxilary stack to the original stack.
*/
void DisplayPatientAddmittions(char *patient_id)
{
    Patient *patient = NULL;
    StackVisits auxStack;
    int visitCntr = 1;
    memset(&auxStack, 0, sizeof(StackVisits));

    patient = searchPatient(patients_tree, patient_id);
    if (patient != NULL) {
        printf ("Patient addmittions:\n");
        printf ("====================\n");
        while (!isStackEmpty(patient->visits)) {
            // stack allows us only to pop nodes, not traverse them.
            // so we will pop each node, and push it back to an auxilary stack. Once finish printing all the data,
            // we will push back the nodes to the original stack.
            Visit* visit = pop(patient->visits);
            printf("Visit %d:\n", visitCntr);
            printf("Arrival time: %d/%d/%d %02d:%02d\n", visit->tArrival.Day, visit->tArrival.Month, visit->tArrival.Year,
                visit->tArrival.Hour, visit->tArrival.Min);
            if (visit->tDismissed.Year != -1) {
                printf("Dismissed time: %d/%d/%d %02d:%02d\n", visit->tDismissed.Day, visit->tDismissed.Month, visit->tDismissed.Year,
                    visit->tDismissed.Hour, visit->tDismissed.Min);
                printf("Duration: %02d:%02d\n", (int)(visit->Duration) / MINUTES_IN_HOUR, (int)(visit->Duration) % MINUTES_IN_HOUR);
            }
            else
            {
                printf("Dismissed time: \n");
                printf("Duration: \n");
            }

            printf("Doctor: %s\n", visit->Doctor->Name);
            printf("Summary: %s\n", visit->vSummary != NULL ? visit->vSummary : "");
            printf("========================\n");
            push(&auxStack, visit);
            visitCntr++;
        }

        // push back the visits to the patient's stack
        while(auxStack.head != NULL) {
            push(patient->visits, pop(&auxStack));
        }
    }
}

/*
remove patient from tree:
To remove a patient from a tree, we need to find it in the tree and then remove it while maintaining the
tree's structure and rules of a binary search tree.
The way we're doing it is to look for the patient, mark it, check it's right child, and then look for the right child's
leftmost children. The ID of this child is the smallest one which is bigger then patient father.
Then, we REPLACE THE DATA of this child with the data of the patient we want to remove, and then remove the child.
If the father of the child had a right son, we will connect the son to the father as its left son.
Them, we will free the child.
Again, we are not replacing the node, but only the data of the node.
 */
void removePatientFromTree(pInTree** root, const char* patient_id) {
    if (*root == NULL) return;
    pInTree* current = *root;
    pInTree* parent = NULL;
    pInTree* p = NULL;
    pInTree* temp;

    // look for the patient
    while ((current != NULL) && (strcmp(current->Patient->ID, patient_id) != 0)) {
        parent = current;
        if (strcmp(patient_id, current->Patient->ID) < 0) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }
    if (current == NULL) return; // Patient not found

    // case of one or no sons
    if (current->left == NULL || current->right == NULL) {
        pInTree* newCurr = (current->left) ? current->left : current->right;
        if (parent == NULL) {
            *root = newCurr;
        }
        else if (parent->left == current) {
            parent->left = newCurr;
        }
        else {
            parent->right = newCurr;
        }
    }
    else {
        // case of two sons
        p = current;
        temp = current->right; // temp point to the right son of the current node
        // look for the leftmost child of the right son
        while (temp->left != NULL)
        {
            p = temp;
            temp = temp->left;
        }

        // Free current's Patient data
        free(current->Patient->Name);
        free(current->Patient->ID);
        free(current->Patient);

        // Move temp's Patient data to current
        current->Patient = temp->Patient;

        // Remove temp from the tree
        if (p != current)
        {
            p->left = temp->right;
        }
        else
        {
            p->right = temp->right;
        }

        // Free temp
        free(temp);
    }
}

/* free tree resources, recurrsivly */
void clearAllPatients(pInTree** root) {
    if (root == NULL || *root == NULL) return;

    // Clear left subtree
    clearAllPatients(&((*root)->left));

    // Clear right subtree
    clearAllPatients(&((*root)->right));

    // Now handle the current node
    if ((*root)->Patient != NULL) {
        if ((*root)->Patient->visits != NULL) {
            Visit* visit;
            while ((*root)->Patient->visits->head != NULL) {
                visit = pop((*root)->Patient->visits);
                if (visit != NULL) {
                    // if patient is treated, decrease the number of patients of the doctor
                    if (visit->tDismissed.Year == -1)
                    {
                        visit->Doctor->nPatients--;
                    }
                    free(visit);
                }
            }
            free((*root)->Patient->visits);
            (*root)->Patient->visits = NULL;
        }

        if ((*root)->Patient->Name != NULL) {
            free((*root)->Patient->Name);
            (*root)->Patient->Name = NULL;
        }
        if ((*root)->Patient->ID != NULL) {
            free((*root)->Patient->ID);
            (*root)->Patient->ID = NULL;
        }
        free((*root)->Patient);
        (*root)->Patient = NULL;
    }

    free(*root);
    *root = NULL;
}

// check number of allergies by counting the number of bits set to 1
static int checkNumOfAllergies(char allergies)
{
    int cntr = 0;
    while (allergies)
    {
        if ((allergies & 0x1) == 0x1)
        {
            cntr++;
        }
        allergies = allergies >> 1;
    }
    return cntr;
}

/* save all the patient allergies to a file */
static void saveAllergies(FILE *file, char patientAllergies)
{
    int maxAllergies = checkNumOfAllergies(patientAllergies);
    int allergiesCntr = 0;
    char allergies;

    if (patientAllergies == NONE)
        fprintf(file, "none");
    else
    {
        // go over each allergy, and see if the patient has it, by bitwise-and it with its allergies (patientAllergies)
        allergies = PENICILLIN;
        while ((allergies <= PRESERVATIVES) && (allergiesCntr <= maxAllergies))
        {
            if (patientAllergies & allergies)
            {
                allergiesCntr++;
                // the last allergy in the list should not be followed by a comma
                if (allergiesCntr < maxAllergies)
                {
                    fprintf(file, "%s,", convertAllergyBitToString(allergies));
                }
                else
                {
                    // this is the last one, no comma needed
                    fprintf(file, "%s", convertAllergyBitToString(allergies));
                    break;
                }

            }
            allergies <<= 1;
        }
    }
    fprintf(file, "\n");
}

/*
save the data from the main db (binary tree) to a file.
As the visit stack is from newest to oldest, we will use an auxilary stack to reverse the order of the visits.
Then we will save the data to the file, and push back the visits to the original stack.
*/
void savePatientQueue() {
    FILE *patients_file = NULL;
    StackVisits auxStack;
    int cntr = 1;
    memset(&auxStack, 0, sizeof(StackVisits));
    patients_file = fopen(PATIENTS_TXT_FILE_PATH, "w");
    if (patients_file == NULL)
    {
        perror("Error opening patients file");
        exit(EXIT_FAILURE);
    }

    fprintf(patients_file, "Name; ID; Allergies\n");
    fprintf(patients_file, "===================\n");

    // we need to save the list of visits, from latest to earliest, so we will use an auxilary stack
    // to reverse the order of the visits stored in the patient's stack. Then, we will save the visit details
    // and push the visit back to the original stack.
    pInLine *p = patients_line;
    while (p != NULL) {
        while (p->lpatient->visits->head != NULL) {
            Visit* visit = pop(p->lpatient->visits);
            push(&auxStack, visit);
        }

        // save name and ID
        fprintf(patients_file, "%d.%s;%s;", cntr, p->lpatient->Name, p->lpatient->ID);
        // save allergies
        saveAllergies(patients_file, p->lpatient->Allergies);
        fprintf(patients_file, "\n");
        // save visits
        while (auxStack.head != NULL)
        {
            Visit* visit = pop(&auxStack);
            fprintf(patients_file, "Arrival:%d/%d/%d %02d:%02d\n", visit->tArrival.Day, visit->tArrival.Month, visit->tArrival.Year,
                    visit->tArrival.Hour, visit->tArrival.Min);
            if (visit->tDismissed.Year == -1)
            {
                fprintf(patients_file, "Dismissed:\n");
                fprintf(patients_file, "Duration:\n");
            }
            else
            {
                fprintf(patients_file, "Dismissed:%d/%d/%d %02d:%02d\n", visit->tDismissed.Day, visit->tDismissed.Month,
                 visit->tDismissed.Year,
                    visit->tDismissed.Hour, visit->tDismissed.Min);
                fprintf(patients_file, "Duration:%02d:%02d\n", (int)(visit->Duration) / MINUTES_IN_HOUR,
                (int)(visit->Duration) % MINUTES_IN_HOUR);
            }

            // save doctor and summary
            fprintf(patients_file, "Doctor:%s\n", visit->Doctor->Name);
            if (visit->vSummary != NULL)
            {
                fprintf(patients_file, "Summary:%s\n", visit->vSummary);
            }
            else
            {
                fprintf(patients_file, "Summary:\n");
            }

            push(p->lpatient->visits, pop(&auxStack));
        }
        fprintf (patients_file, "========================\n");

        // move to next patient
        p = p->next;
        cntr++;
    }
    fclose(patients_file);
}

/* converts allergy bit to string for display */
char *convertAllergyBitToString(char allergy)
{
    if (allergy & PENICILLIN)
        return "Penicillin";

    if (allergy & SULFA)
        return "Sulfa";

    if (allergy & OPIOIDS)
        return "Opioids";

    if (allergy & ANESTHETICS)
        return "Anesthetics";

    if (allergy & EGGS)
        return "Eggs";

    if (allergy & LATEX)
        return "Latex";

    if (allergy & PRESERVATIVES)
    {
        return "Preservatives";
    }
    return NULL;
}
