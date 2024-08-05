#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"


Visit* createVisit(Date tArrival, Date tDismissed, float duration, Doc* doctor, char* vSummary) {
    Visit* visit = (Visit*)calloc(1, sizeof(Visit));
    if (visit == NULL) {
        // handling memory allocation failar
        return NULL;
    }
    visit->Doctor = doctor;
    visit->tArrival = tArrival;
    visit->tDismissed = tDismissed;
    visit->Duration = duration;

    visit->vSummary = NULL;

    return visit;
}


void push(StackVisits *stack, Visit *visit) {
    StackNode* new_node = (StackNode *)calloc(1, sizeof(StackNode));
    if (new_node == NULL) {
        return;
    }

    new_node->visit = visit;
    new_node->next = stack->head;
    stack->head = new_node;
}

Visit *pop(StackVisits *stack)
{
    Visit *data;
    if (stack->head == NULL)
    {
        return NULL;
    }
    StackNode* temp = stack->head;
    data = temp->visit;
    stack->head = stack->head->next;

    free(temp);
    temp = NULL;
    return data;
}

Visit* peek(StackVisits *stack)
{
    return stack->head->visit;
}

void removeVisit(StackVisits *stack)
{
    Date date;
    StackVisits auxStack;
    int found = 0;

    printf ("Please enter the arraival date of the patient, step by step\n");
    getDateFromUser(&date);
    while ((stack->head != NULL) && (found == 0))
    {
        // stack allows us only to pop nodes, not traverse them.
        // so we will pop each node, look for the required visit, if it is not we will push the visit to another stack.
        // Once found, we will free it and push back the visits to the patient stack.
        Visit *visit = pop(stack);
        if (memcmp(&date, &stack->head->visit->tArrival, sizeof(Date)) == 0)
        {
            found = 1;
            // if no dismissed date exist, we also need to remove a patient from the doctor who currently treats it
            // This can probably happen only if this is the top of the addmittions stack
            if (visit->tDismissed.Year == -1)
            {
                visit->Doctor->nPatients--;
            }
            free(visit->vSummary);
            free(visit);
        }
        else
        {
            push(&auxStack, visit);
        }
    } // while

    // now push back the visits to the patient stack
    while (auxStack.head != NULL)
    {
        Visit *visit = pop(&auxStack);
        push(stack, visit);
    }
}
