#ifndef DS3231_H
#define DS3231_H

#include <RTClib.h>

#define ALARM_1 ((uint8_t)1)
#define ALARM_2 ((uint8_t)2)
#define EXT_INT ((uint8_t)7)

extern RTC_DS3231 DS3231;

void setup_ds3231();
void clear_alarms();
bool set_alarm(const DateTime &dt, Ds3231Alarm1Mode alarm_mode);
DateTime now();
DateTime getAlarm1();
Ds3231Alarm1Mode getAlarm1Mode();
void clearAlarm(uint8_t alarm);

#endif // DS3231_H
