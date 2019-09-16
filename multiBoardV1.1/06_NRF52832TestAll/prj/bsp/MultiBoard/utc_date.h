#ifndef __UTC_DATE_H__
#define __UTC_DATA_H__

#include "stdint.h"
#include "boards.h"

#define APP_TIMER_PRESCALER			0
#define UTC_TIMER_OUT				APP_TIMER_TICKS(1000, APP_TIMER_PRESCALER)

// number of seconds since 0 hrs, 0 minutes, 0 seconds, on the
// 1st of January 2000 UTC
typedef uint32_t UTC_SECONDS;

// UTC time structs broken down until standard components.
typedef struct
{
  uint8_t seconds;  // 0-59
  uint8_t minutes;  // 0-59
  uint8_t hour;     // 0-23
  uint8_t day;      // 0-30
  uint8_t month;    // 0-11
  uint16_t year;    // 2000+ 
} UTC_TIME_DEF;


void initUtcDate(void);
UTC_SECONDS getUtcSeconds(void);
void setUtcSeconds(UTC_SECONDS fu32UtcSeconds);
void convertSecondsToUtcTime(UTC_TIME_DEF* ftUtcTime, UTC_SECONDS fu32Second);
UTC_SECONDS convertUtcTimeToSeconds(UTC_TIME_DEF* ftUtcTime);
void displayTime(void);

#endif
