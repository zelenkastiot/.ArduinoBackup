// Arduino DS3232RTC Library
// https://github.com/JChristensen/DS3232RTC
//
// Example sketch illustrating Time library with Real Time Clock.
// This example is identical to the example provided with the Time Library,
// only the #include statement has been changed to include the DS3232RTC library.

#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include "LowPower.h"
#include <Streaming.h>        // http://arduiniana.org/libraries/streaming/

// pin definitions
const uint8_t SQW_PIN(2);
volatile boolean flag = false;
void setup()
{
    Serial.begin(9600);
    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet)
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");
    
    // initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
//    RTC.setAlarm(ALM1_MATCH_DATE, 0, 0, 0, 1);
//    RTC.setAlarm(ALM2_MATCH_DATE, 0, 0, 0, 1);
    RTC.alarm(ALARM_1);
    RTC.alarm(ALARM_2);
    RTC.alarmInterrupt(ALARM_1, false);
    RTC.alarmInterrupt(ALARM_2, false);
    // configure the INT/SQW pin for "interrupt" operation (disable square wave output)
    RTC.squareWave(SQWAVE_NONE);
    pinMode(SQW_PIN, INPUT_PULLUP);
    attachInterrupt(INT0, alarmIsr, FALLING);

    // set Alarm 1 to occur at 6 seconds after every minute
    RTC.setAlarm(ALM1_MATCH_SECONDS, 6, 0, 0, 0);
    // clear the alarm flag
    RTC.alarm(ALARM_1);
    
    // enable interrupt output for Alarm 1
    RTC.alarmInterrupt(ALARM_1, true);

    Serial << millis() << " Start ";
    digitalClockDisplay();
    Serial << endl;
    
}

void loop()
{

      LowPower.powerDown(SLEEP_FOREVER, ADC_ON, BOD_ON); 
    // check to see if the INT/SQW pin is low, i.e. an alarm has occurred
     if (flag)
    {
        if (RTC.alarm(ALARM_1))
        {
            digitalClockDisplay();
            Serial << " --> Alarm 1\n";
        }
       
        flag= false;
    }
}

void alarmIsr()
{
    flag = true;
}
void digitalClockDisplay()
{
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(' ');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(month());
    Serial.print(' ');
    Serial.print(year());
    Serial.println();
}

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}
