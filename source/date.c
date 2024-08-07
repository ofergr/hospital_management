#include "../headers/date.h"

/* Function to create a Date object */
Date createDate(const unsigned int year, const unsigned int month, const unsigned int day, const unsigned int hour,
    const unsigned int minute) {
    Date date;
    date.Year = year;
    date.Month = month;
    date.Day = day;
    date.Hour = hour;
    date.Min = minute;
    return date;
}


/* Function to convert a Date object to minutes. I returns the total minutes from year 0/0/0 00:00.
 For ease of calculation, we assume 30 days in a month and 365 days in a year*/
int convertDateToMinutes(Date* date) {
    int days = 0;

    // add days till year
    days = DAYS_IN_YEAR * date->Year;
    // add days till month
    days += (date->Month - 1) * DAYS_IN_MONTH;
    // add days till day
    days += (date->Day - 1);
    // Convert to minutes
    return (days * 24 * 60) + (date->Hour * 60) + date->Min;
}

/* Function to calculate the time between two dates, in minutes */
float calculateDateDiff(Date* earlyDate, Date* laterDate) {
    int earlyMin = convertDateToMinutes(earlyDate);
    int lateMin = convertDateToMinutes(laterDate);
    int diff = lateMin - earlyMin; // diff in minutes
    return (float)diff;
}


