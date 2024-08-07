#include "../headers/constants.h"
#include "../headers/date.h"
#include "../headers/ptree.h"
#include "../headers/pline.h"
#include "../headers/doctor.h"
#include "../headers/dline.h"
#include "../headers/stackVisits.h"
#include "../headers/patient.h"
#include "../headers/helpers.h"


/* create a visint node to be placed in a visit stack */
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
    if (vSummary != NULL)
    {
        visit->vSummary = calloc(1, MAX_LINE_LENGTH);
        strcpy(visit->vSummary, vSummary);
    }
    else
        visit->vSummary = NULL;

    return visit;
}

/* push a visit to the stack using a wrapper node */
void push(StackVisits *stack, Visit *visit) {
    if (stack == NULL) return;

    StackNode* new_node = (StackNode *)calloc(1, sizeof(StackNode));
    if (new_node == NULL) {
        return;
    }

    new_node->visit = visit;
    new_node->next = stack->head;
    stack->head = new_node;
}

/* pop a visit from the stack */
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

/* peek at the top of the stack for the visit. The node is not popped out */
Visit* peek(StackVisits *stack)
{
    return stack->head->visit;
}

/*
remove a visit from the stack.
As we only allowed to push and pop, we will use a temporary stack to pop all the visits, until we find the
required visit. We then remove it, and push back all the visits then we popped back to the original stack.
*/
void removeVisit(StackVisits *stack)
{
    Date date;
    StackVisits auxStack;
    int found = 0;
    memset(&auxStack, 0, sizeof(StackVisits));
    printf ("Please enter the arraival date of the patient, step by step\n");
    getDateFromUser(&date);
    while ((stack->head != NULL) && (found == 0))
    {
        // stack allows us only to pop nodes, not traverse them.
        // so we will pop each node, look for the required visit, if it is not we will push the visit to another stack.
        // Once found, we will free it and push back the visits to the patient stack.
        Visit *visit = pop(stack);
        if (memcmp(&date, &visit->tArrival, sizeof(Date)) == 0)
        {
            found = 1;
            // if no dismissed date exist, we also need to remove a patient from the doctor who currently treats it
            // This can probably happen only if this is the top of the addmittions stack
            if (visit->tDismissed.Year == -1)
            {
                visit->Doctor->nPatients--;
            }
            if (visit->vSummary != NULL)
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
