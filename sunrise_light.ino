/*
 * Arduino Wake-Up/Sunrise Light v1.0
 * Key Hardware: Arduino Uno/Nano & TinyRTC DS1307 Module
 * Arduino IDE: 1.6.9
 * Based on RTClib Library & Wire Library
 */

#include <RTClib.h> // RTC Library
#include <Wire.h>   // I2C Library

#define LEDPIN 3    // LED Drive O/P =D3
#define SWITCHPIN 2 // LED Switch I/P =D2

#define START_HOUR 5
#define START_MINUTE 30

#define END_HOUR 6
#define END_MINUTE 30

#define OFF_HOUR 7
#define OFF_MINUTE 0

RTC_DS1307 rtc;

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(SWITCHPIN, INPUT);
  digitalWrite(SWITCHPIN, HIGH); // Enable Internal Pull-Up Resistor
  Serial.begin(57600);

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }
  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running!");
    // Sync RTC with the system clock at the time of compilation
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Uncomment – Compile – Upload (See Note)
  }
}

void loop()
{
  DateTime now = rtc.now();
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(1000);

  DateTime startTime = DateTime(now.year(), now.month(), now.day(), START_HOUR, START_MINUTE);
  DateTime endTime = DateTime(now.year(), now.month(), now.day(), END_HOUR, END_MINUTE);

  if (now > startTime & now < endTime)
  {
    Serial.println("On");

    TimeSpan totalTime = endTime - startTime;

    Serial.println(String("totalTime: ") + totalTime.totalseconds());

    TimeSpan timeToEnd = endTime - now;

    Serial.println(String("timeToEnd: ") + timeToEnd.totalseconds());

    float timeRatio = 1 - ((float)timeToEnd.totalseconds() / (float)totalTime.totalseconds());

    Serial.println(String("ratio: ") + timeRatio);

    float adjustedRatio = pow(timeRatio, 3);

    Serial.println(String("adjusted ratio: ") + adjustedRatio);

    int value = 1 + (adjustedRatio * 254);

    Serial.println(String("Value: ") + value);

    analogWrite(LEDPIN, value);
  }
  else
  {
    DateTime offTime = DateTime(now.year(), now.month(), now.day(), OFF_HOUR, OFF_MINUTE);
    if (now > offTime)
    {
      analogWrite(LEDPIN, 0);
    }
    Serial.println("Waiting");
  }

  delay(1000);
}
