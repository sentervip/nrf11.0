#include "utc_date.h"
#include "app_timer.h"
#include "oled.h"
#include "multiboard_process.h"

#define	YearLength(yr)	(IsLeapYear(yr) ? 366 : 365)

#define IsLeapYear(yr)     (!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
   
#define	BEGYEAR	           2000     // UTC started at 00:00:00 January 1, 2000

#define	DAY                86400UL  // 24 hours * 60 minutes * 60 seconds

APP_TIMER_DEF(gtUtcDateTimer);
UTC_SECONDS gu32UtcSeconds = 0;


void displayTime(void)
{
	UTC_TIME_DEF ltUtcTime = {0};
	convertSecondsToUtcTime(&ltUtcTime, getUtcSeconds());
	writeOled5x7ASCII(0x00, 0x0, (ltUtcTime.month + 1) / 10 + 0x30);
	writeOled5x7ASCII(0x01, 0x0, (ltUtcTime.month +1) % 10 + 0x30);

	writeOled5x7ASCII(0x02, 0x0,  '.');
	
	writeOled5x7ASCII(0x03, 0x0, (ltUtcTime.day + 1) / 10 + 0x30);
	writeOled5x7ASCII(0x04, 0x0, (ltUtcTime.day + 1) % 10 + 0x30);

	writeOled5x7ASCII(0x05, 0x0,  ' ');

	writeOled5x7ASCII(0x06, 0x0, ltUtcTime.hour / 10 + 0x30);
	writeOled5x7ASCII(0x07, 0x0, ltUtcTime.hour % 10 + 0x30);
	//if(ltUtcTime.seconds % 2){
		writeOled5x7ASCII(0x08, 0x0, ':');
	//}else{
	//	clrOled8x16TwoDot(0x05, 0x04);
	//}
	writeOled5x7ASCII(0x09, 0x0, ltUtcTime.minutes / 10 + 0x30);
	writeOled5x7ASCII(0x0a, 0x0, ltUtcTime.minutes % 10 + 0x30);
	writeOled5x7ASCII(0x0b, 0x0, ':');
	writeOled5x7ASCII(0x0c, 0x0, ltUtcTime.seconds / 10 + 0x30);
	writeOled5x7ASCII(0x0d, 0x0, ltUtcTime.seconds % 10 + 0x30);
}

void updateSeconds(void* p_context)
{
	gu32UtcSeconds ++;
	if(gtSysVal.b1DispTime)
		displayTime();
}

void initUtcDate(void)
{
	UTC_TIME_DEF ltUtcTime = {0};
	ltUtcTime.year = 2016;
	ltUtcTime.month = 4;
	ltUtcTime.day = 21;
	ltUtcTime.hour = 12;
	ltUtcTime.minutes = 0;
	ltUtcTime.seconds = 0;
	setUtcSeconds(convertUtcTimeToSeconds(&ltUtcTime));

	app_timer_create(&gtUtcDateTimer, APP_TIMER_MODE_REPEATED, updateSeconds);
	app_timer_start(gtUtcDateTimer, UTC_TIMER_OUT, NULL);
}

UTC_SECONDS getUtcSeconds(void)
{
	return gu32UtcSeconds;
}

void setUtcSeconds(UTC_SECONDS fu32UtcSeconds)
{
	gu32UtcSeconds = fu32UtcSeconds;
}

static uint8_t getDaysOfMonth(uint8_t fu8IsLeepYear, uint8_t fu8Mon)
{
  uint8_t days = 31;

  if (fu8Mon == 1) // feb
  {
    days = (28 + fu8IsLeepYear);
  }
  else
  {
    if (fu8Mon > 6) // aug-dec
    {
      fu8Mon --;
    }

    if (fu8Mon & 1)
    {
      days = 30;
    }
  }

  return (days);
}

void convertSecondsToUtcTime(UTC_TIME_DEF* ftUtcTime, UTC_SECONDS fu32Second)
{
  // Calculate the time less than a day - hours, minutes, seconds.
  {
    // The number of seconds that have occured so far stoday.
    uint32_t day = fu32Second % DAY;
    
    // Seconds that have passed in the current minute.
    ftUtcTime->seconds = day % 60UL;
    // Minutes that have passed in the current hour.
    // (seconds per day) / (seconds per minute) = (minutes on an hour boundary)
    ftUtcTime->minutes = (day % 3600UL) / 60UL;
    // Hours that have passed in the current day.
    ftUtcTime->hour = day / 3600UL;
  }

  // Fill in the calendar - day, month, year
  {
    uint16_t numDays = fu32Second / DAY;
    uint8_t monthLen;
    ftUtcTime->year = BEGYEAR;
    
    while (numDays >= YearLength(ftUtcTime->year))
    {
      numDays -= YearLength(ftUtcTime->year);
      ftUtcTime->year++;
    }

    // January.
    ftUtcTime->month = 0;
    
    monthLen = getDaysOfMonth(IsLeapYear(ftUtcTime->year), ftUtcTime->month);
    
    // Determine the number of months which have passed from remaining days.
    while (numDays >= monthLen)
    {
      // Subtract number of days in month from remaining count of days.
      numDays -= monthLen;
      ftUtcTime->month++;
      
      // Recalculate month length.
      monthLen = getDaysOfMonth(IsLeapYear(ftUtcTime->year), ftUtcTime->month);
    }
    
    // Store the remaining days.
    ftUtcTime->day = numDays;
  }
}

UTC_SECONDS convertUtcTimeToSeconds(UTC_TIME_DEF* ftUtcTime)
{
  uint32_t seconds;

  // Seconds for the partial day.
  seconds = (((ftUtcTime->hour * 60UL) + ftUtcTime->minutes) * 60UL) + ftUtcTime->seconds;

  // Account for previous complete days.
  {
    // Start with complete days in current month.
    uint16_t days = ftUtcTime->day;

    // Next, complete months in current year.
    {
      int8_t month = ftUtcTime->month;
      while (--month >= 0)
      {
        days += getDaysOfMonth(IsLeapYear(ftUtcTime->year), month);
      }
    }

    // Next, complete years before current year.
    {
      uint16_t year = ftUtcTime->year;
      while (--year >= BEGYEAR)
      {
        days += YearLength(year);
      }
    }

    // Add total seconds before partial day.
    seconds += (days * DAY);
  }

  return (seconds);
}
