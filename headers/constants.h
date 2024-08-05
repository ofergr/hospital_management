/* text files paths */
#define DOCTORS_TXT_FILE_PATH "Doctors.txt"
#define PATIENTS_TXT_FILE_PATH "Patients.txt"
#define PATIENTS_LINE_TXT_FILE_PATH "Line.txt"

#define MAX_LINE_LENGTH 256

/* menu options */
#define EXIT_PROGRAM 0
#define ADMIT_PATIENT 1
#define CHECK_FOR_PATIENTS_ALLERGIES 2
#define DISPLAY_ALL_PATIENTS 3
#define DISPLAY_ALL_PATIENTS_ADDMITIONS 4
#define DISPLAY_ALL_PATIENTS_IN_LINE 5
#define ADVANCE_PATIENT_IN_LINE 6
#define DISPLAY_LIST_OF_DOCTORS 7
#define DISPLAY_ALL_PATIENTS_ASSIGNED_TO_A_DOCTOR 8
#define DISCHARGE_PATIENT 9
#define REMOVE_VISIT 10
#define REMOVE_PATIENT 11
#define CLOSE_THE_HOSPITAL 12

/* allergies codes */
#define NONE 0x00
#define PENICILLIN 0x01
#define SULFA 0x02
#define OPIOIDS 0x04
#define ANESTHETICS 0x08
#define EGGS 0x10
#define LATEX 0x20
#define PRESERVATIVES 0x40
#define ILEGAL_allergIES 0xff

/* error messgaes */
#define ERROR_NO_DOCTOR_AVAILLEABLE "No doctor available at the moment, Please come later\n"
#define ERROR_PATIENT_NOT_FOUND "The patient with the specified id is not exists in the database\n"
