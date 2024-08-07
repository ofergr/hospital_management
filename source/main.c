#include <stdio.h>
#include <stdlib.h>
#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"

void exitProgram();

/*
main function:
Loads details from all the files, and populate the relevant data bases
Displays the menu and wait for user input
Uopn exit, save all data to files and clear all memory
*/
int main(int argc, char **argv) {
    int user_choice = 0;

    char patient_id[MAX_LINE_LENGTH] = {0};
    char doctorName[MAX_LINE_LENGTH] = { 0 };
    Patient *patient = NULL;

    loadDoctors();
    loadPatients();
    loadPatientsLine();

    do {
        printMenu();
        scanf("%d", &user_choice);
        getchar();
        printf("\n");
        switch (user_choice) {
            case EXIT_PROGRAM:
            break;
            case ADMIT_PATIENT:
                // check if there is an available doctor
                if (isDoctorsAvailable() != NULL) {
                    // admint patient
                    admitNewPatient(patients_tree);
                }
                else
                {
                    printf(ERROR_NO_DOCTOR_AVAILLEABLE);
                }
            break;
            case CHECK_FOR_PATIENTS_ALLERGIES:
                displayPatients();
                getUserId(patient_id);
                patient = searchPatient(patients_tree, patient_id);
                memset(patient_id, '\0', MAX_LINE_LENGTH);
                if (patient != NULL) {
                    printf("Patient allergies:");
                    printPatientAllergies(patient);
                } else {
                    printf("%s\n", ERROR_PATIENT_NOT_FOUND);
                    }
            break;
            case DISPLAY_ALL_PATIENTS:
                printf("List of all patients\n");
                printf("=====================\n");
                displayAllPatients(patients_tree);
                printf("\n");
                break;
            case DISPLAY_ALL_PATIENTS_ADDMITIONS:
                displayPatients();
                getUserId(patient_id);
                DisplayPatientAddmittions(patient_id);
                printf("\n");
                break;
            case DISPLAY_ALL_PATIENTS_IN_LINE:
                displayPatientsInLine();
                printf("\n\n");
                break;
            case ADVANCE_PATIENT_IN_LINE:
                displayPatients();
                getUserId(patient_id);
                patients_line = advancePatientInLine(patients_line, patient_id);
                memset(patient_id, '\0', MAX_LINE_LENGTH);
                displayPatients();
                printf("\n\n");
            break;
            case DISPLAY_LIST_OF_DOCTORS:
                printDoctorList();
                printf("\n");
            break;
            case DISPLAY_ALL_PATIENTS_ASSIGNED_TO_A_DOCTOR:
                int valid = 0;
                printDoctorList();
                while (!valid) {
                    printf("Please enter Doctor's name: \n");
                    fgets(doctorName, MAX_LINE_LENGTH, stdin);
                    doctorName[strlen(doctorName) - 1] = '\0';
                    if (getDoctorByName(doctorName) != NULL)
                    {
                        valid = 1;
                    }
                    else
                    {
                        printf("Doctor not found, please try again\n");
                    }
                }
                printf ("\n");
                DispalyDoctorPatients(patients_line, doctorName);
                break;
            case DISCHARGE_PATIENT:
                displayPatients();
                printf("\n");
                dischargePatient();
                printf("\nUpdated patients line: \n");
                displayPatients();
            break;
            case REMOVE_VISIT:
                displayPatients();
                printf("\n");
                getUserId(patient_id);
                patient = searchPatient(patients_tree, patient_id);
                if (patient != NULL) {
                    printf("\n");
                    DisplayPatientAddmittions(patient_id);
                    removeVisit(patient->visits);
                    DisplayPatientAddmittions(patient_id);
                    printf("\n");
                } else {
                    printf("%s\n", ERROR_PATIENT_NOT_FOUND);
                }
                break;
            case REMOVE_PATIENT:
                displayPatients();
                getUserId(patient_id);
                removePatientFromLine(&patients_line, patient_id);
                removePatientFromTree(&patients_tree, patient_id);
                printf("Patient removed successfully.\n");
                break;
            case CLOSE_THE_HOSPITAL:
                printf("Closing hospital...\n");
                clearAllPatients(&patients_tree);
                clearAllPatientsInLine(&patients_line);
                freeDoctorList();
                printf("All data was deleted. Goodbye!\n");
                exit(0);
            default:
                printf("Illegal choice, please try again...\n");
                break;
        }
    } while (user_choice != EXIT_PROGRAM);
    exitProgram();
    return 0;

}

/* exit program: save data to files and clear the memory */
void exitProgram()
{
    printf("Exiting program: saving data to files and clearing all memory\n");
    saveDoctorList();
    savePatientQueue();
    savePatientLine();
    clearAllPatients(&patients_tree);
    clearAllPatientsInLine(&patients_line);
    freeDoctorList();
    printf("Goodbye\n");
}
