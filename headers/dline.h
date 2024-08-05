#ifndef DOCTOR_LINE_H
#define DOCTOR_LINE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct doctors_list_ {
    Doc *ldoctor;
    struct doctors_list_ *next;
} doctors_list;

extern doctors_list *doctors_list_head;

void insertDoctorToList(Doc *doctor);

Doc *getDoctorByName(const char *name);

void printDoctorList();

void freeDoctorList();

#endif
