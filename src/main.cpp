#include <Constants.h>
#include <I2C.h>
#include <Wire.h>
#include <Data.h>
#include <Sensor.h>
#include <BME280.h>
#include <WiFiFunctions.h>

void setup()
{
  Serial.begin(76800);
  Serial.setTimeout(2000);
  Serial.setDebugOutput(false);
  Serial.println("setup");
  while (!Serial)
  {
  }

  pinMode(LED_BUILTIN, INPUT);
  pinMode(SENSOR_PWR, OUTPUT);
  pinMode(WAKEUP, WAKEUP_PULLUP);

  digitalWrite(0U, LOW);
  digitalWrite(12U, LOW);
  digitalWrite(13U, LOW);
  digitalWrite(15U, LOW);
  ESP.wdtEnable(60000U);

  digitalWrite(LED_BUILTIN, LOW);
  Wire.begin();

  // scanI2C();
  readSensors();
  if (voltage > 3.0F)
  {
    sendDataDomoticz();
  }
  hibernate();
}

void loop()
{
  Serial.println("loop");
  delay(100);
}
