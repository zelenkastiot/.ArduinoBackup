#include <DS3232RTC.h>        // https://github.com/JChristensen/DS3232RTC
#include <LowPower.h>
#include "RTClib.h"
#include "PinChangeInterrupt.h"  //https://github.com/NicoHood/PinChangeInterrupt

RTC_DS3231 rtc;

#define rtcAlarmPin 10 // External interrupt on pin D3
volatile bool alarmNow = false;
void setup()
{
  pinMode(22, OUTPUT);
  digitalWrite(22, LOW);
  Serial.begin(115200);
  pinMode(rtcAlarmPin, INPUT_PULLUP); // Set interrupt pin
  attachPCINT(digitalPinToPCINT(rtcAlarmPin), wake, FALLING);
  // initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
  RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
  RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
  RTC.alarm(ALARM_1);
  RTC.alarm(ALARM_2);
  RTC.alarmInterrupt(ALARM_1, false);
  RTC.alarmInterrupt(ALARM_2, false);
  RTC.squareWave(SQWAVE_NONE);

  // set the alarm
  DateTime now = rtc.now();
  Serial.println(F("Current Time: "));
  printDateTime(now);
  DateTime future (now + TimeSpan(0, 0, 0, 15)); //Tage, Stunden, Minuten, Sekunden
  Serial.println(F("Alarm is set to: "));
  printDateTime(future);
  
  RTC.setAlarm(ALM1_MATCH_DATE, future.second(), future.minute(), future.hour(), future.day());
  // clear the alarm flag
  RTC.alarm(ALARM_1);
  RTC.alarmInterrupt(ALARM_1, true); // Enable alarm 1 interrupt A1IE
}

void loop()
{
  sleepNow(); // Enter slee digitalPinToInterrupt(2)
  setRTCagain();
}
// Interrupt Service Routine (ISR) function
void wake ()
{
  // detachInterrupt (INT2); // Disable interrupts on D3
}

void sleepNow ()
{
  noInterrupts (); // Disable interrupts before entering sleep
  // attachInterrupt (INT2, wake, FALLING);  // Wake on falling edge of D3
  interrupts (); // Enable interrupts to ensure next instruction is executed
  Serial.println("Entering sleep");
  delay(20);
  Serial.flush();
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

void setRTCagain() {
  Serial.println("Awake...");

  if ( RTC.alarm(ALARM_1) )
  {
    Serial.println("Now Alarm!");
    // set the alarm
    DateTime now = rtc.now();
    DateTime future (now + TimeSpan(0, 0, 0, 15)); //Days, Hours, Minutes, Seconds
    Serial.println(F("Alarm is set to: "));
    printDateTime(future);
    RTC.setAlarm(ALM1_MATCH_DATE, future.second(), future.minute(), future.hour(), future.day());
  }
}

void printDateTime(DateTime t)
{
  Serial.print((t.day() < 10) ? "0" : ""); Serial.print(t.day(), DEC); Serial.print('/');
  Serial.print((t.month() < 10) ? "0" : ""); Serial.print(t.month(), DEC); Serial.print('/');
  Serial.print(t.year(), DEC); Serial.print(' ');
  Serial.print((t.hour() < 10) ? "0" : ""); Serial.print(t.hour(), DEC); Serial.print(':');
  Serial.print((t.minute() < 10) ? "0" : ""); Serial.print(t.minute(), DEC); Serial.print(':');
  Serial.print((t.second() < 10) ? "0" : ""); Serial.println(t.second(), DEC);
}
