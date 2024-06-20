#include <DS3231.h>

RTC_DS3231 DS3231;

void setup_ds3231(){
  if(!DS3231.begin()) {
    Serial.println("Couldn't find RTC!");
    Serial.flush();
    while (1) delay(10);
  }

  if(DS3231.lostPower()) {
    // this will adjust to the date and time at compilation
    DS3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void clear_alarms(){
  DS3231.disable32K();

  // Making it so, that the alarm will trigger an interrupt

  // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
  // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
  DS3231.clearAlarm(1);
  DS3231.clearAlarm(2);

  // turn off alarm 2 (in case it isn't off already)
  // again, this isn't done at reboot, so a previously set alarm could easily go overlooked
  DS3231.disableAlarm(2);

  // stop oscillating signals at SQW Pin
  // otherwise setAlarm1 will fail
  DS3231.writeSqwPinMode(DS3231_OFF);
}

bool set_alarm(const DateTime &dt, Ds3231Alarm1Mode alarm_mode){
  if(!DS3231.setAlarm1(dt, alarm_mode)) {
    Serial.println("Error, alarm wasn't set!");
    return false;
  }else {
    Serial.println("Alarm will happen in 10 seconds!");
    return true;
  }
}

DateTime now(){
  return DS3231.now();
}

DateTime getAlarm1(){
  return DS3231.getAlarm1();
}

Ds3231Alarm1Mode getAlarm1Mode(){
  return DS3231.getAlarm1Mode();
}

void clearAlarm(uint8_t alarm){
  DS3231.clearAlarm(alarm);
}