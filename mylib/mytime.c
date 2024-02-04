#include <stdio.h>
#include <time.h>

void displayCurrentTime()
{
    time_t currentTime;
    struct  tm *localTime;
    char timeString[80];

    currentTime = time(NULL);
    localTime = localtime(&currentTime);

    strftime(timeString, 80, "%Y-%m-%d %H:%M:%S", localTime);

    printf("Time:%s.\n", timeString);
    
}