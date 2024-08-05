#ifndef DATE_H
#define DATE_H

#include <stdio.h>
#include <time.h>

#define DAYS_IN_MONTH 30
#define DAYS_IN_YEAR 365
#define MINUTES_IN_HOUR 60

typedef struct Date {
    unsigned int Year;
    unsigned int Month;
    unsigned int Day;
    unsigned int Hour;
    unsigned int Min;
} Date;

Date createDate(const unsigned int year, const unsigned int month, const unsigned int day, const unsigned int hour, 
    const unsigned int minute);
int convertDateToMinutes(Date* date);
float calculateDateDiff(Date* earlyDate, Date* laterDate);
#endif
