#ifndef STACK_VISITS_H
#define STACK_VISITS_H

#include <stdio.h>

typedef struct Visit {
    Date tArrival;
    Date tDismissed;
    float Duration;
    Doc *Doctor;
    char *vSummary;
} Visit;

typedef struct StackNode {
    Visit *visit;
    struct StackNode *next;
} StackNode;

typedef struct StackVisits {
    StackNode *head;
} StackVisits;

Visit *createVisit(Date tArrival, Date tDismissed, float Duration, Doc *Doctor, char *vSummary);
void push(StackVisits *stack, Visit *visit);
Visit *pop(StackVisits *stack);
Visit *peek(StackVisits *stack);
void removeVisit(StackVisits *stack);

#endif