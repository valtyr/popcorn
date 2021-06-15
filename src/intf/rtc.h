#pragma once

#include "isr.h"
#include "io.h"

typedef struct RTCTime
{
    int seconds;
    int minutes;
    /* 24 hour time format */
    int hours;
    int weekday;
    int day;
    int month;
    int year;
} RTCTime;

void RTCInit();
RTCTime RTCNow();
