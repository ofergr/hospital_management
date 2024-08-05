#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_ID_LEN 9

void loadDoctors();
void analyzeDoctorsLine(char buffer[MAX_LINE_LENGTH]);

void loadPatients();
void analyzePatientsLine(char buffer[MAX_LINE_LENGTH], FILE *patients_file);

void loadPatientsQueue();

char extractAllergies(char *allergies);

Date extractDateFronString(char *str);

Doc *isDoctorsAvailable();

void printPatientAllergies(Patient *patient);

void printMenu();

Doc* findAvailableDoc();

char getallergiesFromUser();

Doc* assignDoctor2Case();

void getDateFromUser(Date* date);

void getUserId(char *str);

int isNumber(char *str);

#endif
