#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"

void loadDoctors() {
    FILE *doctors_file = NULL;
    int line_count = 0;

    doctors_file = fopen(DOCTORS_TXT_FILE_PATH, "r");
    char buffer[MAX_LINE_LENGTH] = {0};

    if (doctors_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), doctors_file)) {
        line_count++;
        buffer[strlen(buffer) - 1] = '\0';
        if (line_count >= 3) {
            analyzeDoctorsLine(buffer);
        }
        memset(buffer, '\0', MAX_LINE_LENGTH);
    }

    fclose(doctors_file);
}

void analyzeDoctorsLine(char buffer[MAX_LINE_LENGTH]) {
    char copy_of_buffer[MAX_LINE_LENGTH] = {0};
    char *token = NULL;
    char *doctor_name = NULL;
    char *doctor_license = NULL;
    unsigned int doctor_patients = 0;

    strcpy(copy_of_buffer, buffer);

    token = strtok(copy_of_buffer, ";");
    doctor_name = token;

    token = strtok(NULL, "; ");
    doctor_license = token;

    token = strtok(NULL, "");
    doctor_patients = atoi(token);

    insertDoctorToList(createDoctor(doctor_name, doctor_license, doctor_patients));
}

void loadPatients() {
    FILE *patients_file = NULL;
    patients_file = fopen(PATIENTS_TXT_FILE_PATH, "r");
    char buffer[MAX_LINE_LENGTH] = {0};
    Patient *patient = NULL;
    if (patients_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    /* skipping the first 2 line in the file */
    fgets(buffer, sizeof(buffer), patients_file);
    fgets(buffer, sizeof(buffer), patients_file);
    while (fgets(buffer, sizeof(buffer), patients_file)) {
        buffer[strlen(buffer) - 1] = '\0';
        if (buffer[0] != '\0' && buffer[0] != '\r') {
            if (buffer[0] =='=')
            {
                patient = NULL;
            }
            else
                analyzePatientsLine(buffer, patients_file, &patient);
        }
        memset(buffer, '\0', MAX_LINE_LENGTH);
    }
    fclose(patients_file);
}

void analyzePatientsLine(char buffer[MAX_LINE_LENGTH], FILE *patients_file, Patient **patient)
{
    char copy_of_buffer[MAX_LINE_LENGTH] = {0};
    char *copy_of_buffer_without_number = copy_of_buffer + 2;
    char *token = NULL;
    char *patient_name = NULL;
    char *patient_id = NULL;
    char patient_allergies;
    char *summary = NULL;
    Doc *assigned_doctor = NULL;
    Date arrival_date, dissmised_date;
    Visit *visit = NULL;

    strcpy(copy_of_buffer, buffer);

    /* if a new patient is reached */
    if (isdigit(buffer[0])) {
        token = strtok(copy_of_buffer_without_number, ";");
        patient_name = token;

        token = strtok(NULL, ";");
        patient_id = token;

        token = strtok(NULL, ";");
        token[strlen(token)] = '\0';
        patient_allergies = extractAllergies(token);
        *patient = createPatient(patient_name, patient_id, patient_allergies, 0);
        patients_tree = insertPatientToTree(patients_tree, *patient);
    } else {
        float duration = 0;

        // copy_of_buffer[strlen(copy_of_buffer) - 1] = '\0';
        arrival_date = extractDateFronString(copy_of_buffer);
        memset(copy_of_buffer, '\0', strlen(copy_of_buffer));

        fgets(copy_of_buffer, sizeof(copy_of_buffer), patients_file);
        copy_of_buffer[strlen(copy_of_buffer) - 2] = '\0';
        if (strcmp(copy_of_buffer, "Dismissed:") || strcmp(copy_of_buffer, "Dismissed")) {
            dissmised_date = extractDateFronString(copy_of_buffer);
        }
        memset(copy_of_buffer, '\0', strlen(copy_of_buffer));

        fgets(copy_of_buffer, sizeof(copy_of_buffer), patients_file);
        copy_of_buffer[strlen(copy_of_buffer) - 2] = '\0';
        if (strcmp(copy_of_buffer, "Duration:")) {
            token = strtok(copy_of_buffer, ":");
            token = strtok(NULL, ":");
            if (token != NULL) {
                duration = (float)atoi(token) * MINUTES_IN_HOUR;
                token = strtok(NULL, ":");
                duration += (float)atoi(token);
            }
        }
        memset(copy_of_buffer, '\0', strlen(copy_of_buffer));

        fgets(copy_of_buffer, sizeof(copy_of_buffer), patients_file);
        token = strtok(copy_of_buffer, ":");
        token = strtok(NULL, "");
        token[strlen(token) - 1] = '\0';
        assigned_doctor = getDoctorByName(token);
        memset(copy_of_buffer, '\0', strlen(copy_of_buffer));

        fgets(copy_of_buffer, sizeof(copy_of_buffer), patients_file);
        copy_of_buffer[strlen(copy_of_buffer) - 2] = '\0';
        if (strcmp(copy_of_buffer, "Summary:")) {
            token = strtok(copy_of_buffer, ":");
            token = strtok(NULL, ":");
            if (token != NULL) {
                summary = (char*)calloc(strlen(token) + 1, sizeof(char));
                strcpy(summary, token);
                memset(copy_of_buffer, '\0', strlen(copy_of_buffer));
            } else {
                summary = NULL;
            }
        }

        if (duration == 0)
            duration = -1;
        visit = createVisit(arrival_date, dissmised_date, duration, assigned_doctor, summary);
        push((*patient)->visits, visit);
    }
}

void loadPatientsLine()
{
    FILE *patients_line_file = NULL;
    patients_line_file = fopen(PATIENTS_LINE_TXT_FILE_PATH, "r");
    char buffer[MAX_LINE_LENGTH] = {0};
    char *patient_id = NULL;
    Patient *patient = NULL;

    if (patients_line_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), patients_line_file)) {
        buffer[strlen(buffer) - 1] = '\0';
        if (isdigit(buffer[0])) {
            patient_id = buffer + 2;
            patient = searchPatient(patients_tree, patient_id);
            if (patient == NULL)
                continue;
            insertPatientToLine(&patients_line, patient);
        }
        memset(buffer, '\0', MAX_LINE_LENGTH);
    }
    fclose(patients_line_file);
}

char extractAllergies(char *allergies)
{
    char *token = NULL;
    char allergies_options = NONE;

    token = strtok(allergies, ",");
    while (token != NULL) {
        if (strcmp(token, "Penicillin") == 0) {
            allergies_options |= PENICILLIN;
        } else if (strcmp(token, "Sulfa") == 0) {
            allergies_options |= SULFA;
        } else if (strcmp(token, "Opioids") == 0) {
            allergies_options |= OPIOIDS;
        } else if (strcmp(token, "Anesthetics") == 0) {
            allergies_options |= ANESTHETICS;
        } else if (strcmp(token, "Eggs") == 0) {
            allergies_options |= EGGS;
        } else if (strcmp(token, "Latex") == 0) {
            allergies_options |= LATEX;
        } else if (strcmp(token, "Preservatives") == 0) {
            allergies_options |= PRESERVATIVES;
        }
        token = strtok(NULL, ",");
    }

    return allergies_options;
}

Doc* assignDoctor2Case()
{
    Doc* doctor = findAvailableDoc();
    if (doctor != NULL)
    {
        doctor->nPatients += 1;
        return doctor;
    }
    return NULL;
}

char getAllergiesFromUser() {
    char allergy[MAX_LINE_LENGTH] = { 0 };
    char yesNo[MAX_LINE_LENGTH] = { 0 };
    char allergies = 0x0;

    printf("Do you have any allergy (y/n)?\n");
    fgets(yesNo, MAX_LINE_LENGTH, stdin);
    while ((yesNo[0] == 'Y') || (yesNo[0] == 'y'))
    {
        printf("Please enter allergy name\n");
        fgets(allergy, MAX_LINE_LENGTH, stdin);
        memset(yesNo, 0, MAX_LINE_LENGTH);
        while
            (
                (!strcmp(allergy, "Penicillin")) && (!strcmp(allergy, "Sulfa")) && (!strcmp(allergy, "Opioids")) &&
                (!strcmp(allergy, "Anesthetics")) && (!strcmp(allergy, "Eggs")) && (!strcmp(allergy, "Latex")) &&
                (!strcmp(allergy, "Preservatives"))
                )
        {
            printf("allergy name not exist, please try again\n");
        }

        if (strcmp(allergy, "Penicillin") == 0) {
            allergies |= PENICILLIN;
        }
        else if (strcmp(allergy, "Sulfa") == 0) {
            allergies |= SULFA;
        }
        else if (strcmp(allergy, "Opioids") == 0) {
            allergies |= OPIOIDS;
        }
        else if (strcmp(allergy, "Anesthetics") == 0) {
            allergies |= ANESTHETICS;
        }
        else if (strcmp(allergy, "Eggs") == 0) {
            allergies |= EGGS;
        }
        else if (strcmp(allergy, "Latex") == 0) {
            allergies |= LATEX;
        }
        else if (strcmp(allergy, "Preservatives") == 0) {
            allergies |= PRESERVATIVES;
        }

        memset(allergy, 0, MAX_LINE_LENGTH);
        printf("Do you have another allergy?\n");
        fgets(yesNo, MAX_LINE_LENGTH, stdin);
    }

    return allergies;
}

Date extractDateFronString(char* str) {
    unsigned int date_information[5] = { 0 };
    char* tok = strtok(str, ":");
    int i = 0;

    tok = strtok(NULL, "/");
    if (tok == NULL) {
        return createDate(-1, -1, -1, -1, -1);
    }
    date_information[0] = atoi(tok);

    tok = strtok(NULL, "/");
    date_information[1] = atoi(tok);

    tok = strtok(NULL, " ");
    date_information[2] = atoi(tok);

    tok = strtok(NULL, ":");
    date_information[3] = atoi(tok);

    tok = strtok(NULL, "");
    date_information[4] = atoi(tok);

    return createDate(date_information[2], date_information[1], date_information[0], date_information[3], date_information[4]);
}

Doc *isDoctorsAvailable() {
    doctors_list *p = doctors_list_head;

    while (p != NULL) {
        if (p->ldoctor->nPatients < MAX_PATIENT_PER_DOC)
        {
            return p->ldoctor;
        }
        else
            p = p->next;
    }
    return NULL;
}

Doc* findAvailableDoc() {
    doctors_list* p = doctors_list_head;
    unsigned int minPatient = MAX_PATIENT_PER_DOC;
    Doc* availableDoc = NULL;

    while (p != NULL) {
        if (p->ldoctor->nPatients < minPatient) {
            availableDoc = p->ldoctor;
            minPatient = p->ldoctor->nPatients;
        }
        p = p->next;
    }
    return availableDoc;
}

void printPatientAllergies(Patient *patient) {
    if (patient->Allergies & NONE) {
        printf(" None");
    }

    if (patient->Allergies & PENICILLIN) {
        printf(" Penicillin");
    }

    if (patient->Allergies & SULFA) {
        printf(" Sulfa");
    }

    if (patient->Allergies & OPIOIDS) {
        printf(" Opioids");
    }

    if (patient->Allergies & ANESTHETICS) {
        printf(" Anesthetics");
    }

    if (patient->Allergies & EGGS) {
        printf(" Eggs");
    }

    if (patient->Allergies & LATEX) {
        printf(" Latex");
    }

    if (patient->Allergies & PRESERVATIVES) {
        printf(" Preservatives");
    }
    printf("\n\n\n");
}

void getDateFromUser(Date *date) {
    char str[MAX_LINE_LENGTH] = { 0 };
    int valid = 0, num;
    printf("Please enter year (1900-2200):\n");
    while (!valid)
    {
        fgets(str, MAX_LINE_LENGTH, stdin);
        str[strlen(str) - 1] = '\0';
        num = atoi(str);
        if ((num < 1900) || (num > 2200) || !isNumber(str))
        {
            printf("Invalid year (%s), please enter again\n", str);
        }
        else
        {
            valid = 1;
            date->Year = num;
            memset(str, 0, MAX_LINE_LENGTH);
        }
    }

    valid = 0;
    printf("Please enter month (1-12):\n");
    while (!valid)
    {
        fgets(str, MAX_LINE_LENGTH, stdin);
        str[strlen(str) - 1] = '\0';
        num = atoi(str);
        if ((num < 1) || (num>12) || !isNumber(str))
        {
            printf("Invalid month (%s), please enter again\n", str);
        }
        else
        {
            valid = 1;
            date->Month = num;
            memset(str, 0, MAX_LINE_LENGTH);
        }
    }

    valid = 0;
    printf("Please enter day (1-30):\n");
    while (!valid)
    {
        fgets(str, MAX_LINE_LENGTH, stdin);
        str[strlen(str) - 1] = '\0';
        num = atoi(str);
        if ((num < 1) || (num > 30) || !isNumber(str))
        {
            printf("Invalid day (%s), please enter again\n", str);
        }
        else
        {
            valid = 1;
            date->Day = num;
            memset(str, 0, MAX_LINE_LENGTH);
        }
    }

    valid = 0;
    printf("Please enter hour (0-23):\n");
    while (!valid)
    {
        fgets(str, MAX_LINE_LENGTH, stdin);
        str[strlen(str) - 1] = '\0';
        num = atoi(str);
        if ((num < 0) || (num > 23) || !isNumber(str))
        {
            printf("Invalid hour (%s), please enter again\n", str);
        }
        else
        {
            valid = 1;
            date->Hour = num;
            memset(str, 0, MAX_LINE_LENGTH);
        }
    }

    valid = 0;
    printf("Please enter minutes (0-59):\n");
    while (!valid)
    {
        fgets(str, MAX_LINE_LENGTH, stdin);
        str[strlen(str) - 1] = '\0';
        num = atoi(str);
        if ((num < 0) || (num > 59) || !isNumber(str))
        {
            printf("Invalid minutes (%s), please enter again\n", str);
        }
        else
        {
            valid = 1;
            date->Min = num;
            memset(str, 0, MAX_LINE_LENGTH);
        }
    }
    return;
}

int isNumber(char* str) {
    while (*str) {
        if (*str < '0' || *str > '9')
            return 0;
        ++str;
    }
    return 1;
}

void getUserId(char* str)
{
    int valid = 0;
    if (str == NULL)
        return;
    printf("please enter a 9-digits ID number: \n");
    while (valid == 0)
    {
        fgets(str, MAX_LINE_LENGTH, stdin);
        str[strlen(str) - 1] = '\0';
        if ((strlen(str) != MAX_ID_LEN) || !isNumber(str))
        {
            printf("Invalid ID number (%s), please enter again\n", str);
            memset(str, 0, MAX_LINE_LENGTH);
        }
        else
        {
            valid = 1;
        }
    }
}

void printMenu() {
    printf("0. Exit program\n");
    printf("1. admit patient\n");
    printf("2. check for patients allergies\n");
    printf("3. display all patients\n");
    printf("4. display all patients addmissions\n");
    printf("5. display all patients in line\n");
    printf("6. advance patients in line\n");
    printf("7. display list of doctors\n");
    printf("8. display all patients assigned to a doctor\n");
    printf("9. discharge patient\n");
    printf("10. remove visit\n");
    printf("11. remove patient\n");
    printf("12. close the hospital\n");
    printf("Please enter you choice: ");
}

