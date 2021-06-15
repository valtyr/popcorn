#include "rtc.h"

#define CURRENT_YEAR 2021

typedef enum
{
    CMOS_R_SECONDS = 0x00,
    CMOS_R_MINUTES = 0x02,
    CMOS_R_HOURS = 0x04,
    CMOS_R_WEEKDAY = 0x06,
    CMOS_R_DAY_OF_MONTH = 0x07,
    CMOS_R_MONTH = 0x08,
    CMOS_R_YEAR = 0x09,
    CMOS_R_CENTURY = 0x32,
    CMOS_R_STATUS_A = 0x0A,
    CMOS_R_STATUS_B = 0x0B,
} CMOSRegister;

int read_cmos(CMOSRegister r)
{
    outb(0x70, r);
    return inb(0x71);
}

int update_in_progress()
{
    return read_cmos(CMOS_R_STATUS_A) & 0x80;
}

RTCTime RTCNow()
{

    while (update_in_progress())
    {
        // Chill for a bit

        // We could get bad state here...
        // might want to read the registers
        // twice until they match.
    }

    int seconds = read_cmos(CMOS_R_SECONDS);
    int minutes = read_cmos(CMOS_R_MINUTES);
    int hours = read_cmos(CMOS_R_HOURS);
    int day = read_cmos(CMOS_R_DAY_OF_MONTH);
    int month = read_cmos(CMOS_R_MONTH);
    int year = read_cmos(CMOS_R_YEAR);
    int century = read_cmos(CMOS_R_CENTURY);

    int registerB = read_cmos(CMOS_R_STATUS_B);

    seconds = (seconds & 0x0F) + ((seconds / 16) * 10);
    minutes = (minutes & 0x0F) + ((minutes / 16) * 10);
    hours = ((hours & 0x0F) + (((hours & 0x70) / 16) * 10)) | (hours & 0x80);
    day = (day & 0x0F) + ((day / 16) * 10);
    month = (month & 0x0F) + ((month / 16) * 10);
    year = (year & 0x0F) + ((year / 16) * 10);
    if (century != 0)
    {
        century = (century & 0x0F) + ((century / 16) * 10);
        year += century * 100;
    }
    else
    {
        year += (CURRENT_YEAR / 100) * 100;
        if (year < CURRENT_YEAR)
            year += 100;
    }

    if (!(registerB & 0x02) && (hours & 0x80))
    {
        hours = ((hours & 0x7F) + 12) % 24;
    }

    return (RTCTime){
        .seconds = seconds,
        .minutes = minutes,
        .hours = hours,
        .day = day,
        .month = month,
        .year = year,
    };
}

void RTCInit()
{
    BIOSPrintf("Initializing RTC...\n");

    cli();

    outb(0x70, 0x8A); // Select status register A and disable NMI
    outb(0x71, 0x20); // Write to CMOS/RTC RAM

    sti();

    RTCTime currentTime = RTCNow();
    BIOSPrintf(
        "RTC initialized at %d:%02d:%02d %02d/%02d/%d\n",
        currentTime.hours,
        currentTime.minutes,
        currentTime.seconds,
        currentTime.day,
        currentTime.month + 1,
        currentTime.year);
}
