#include <SPI.h>
#include <Sensor.h>
#include <Data.h>
#include <Constants.h>
#include <BME280.h>

Adafruit_ADS1115 ads;
int soilMoisture;
int vcc;

void readSensors()
{
  digitalWrite(SENSOR_PWR, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Sensor power on.");
  delay(1000);

  if (!BME280init())
  {
    Serial.println("Failed to initialize BME280.");
  }
 
  if (!ads.begin())
  {
    Serial.println("Failed to initialize ADS.");
  }

  BME280read();
  soilMv = (int) (ads.computeVolts(ads.readADC_SingleEnded(0)) * 1000.0F);
  lux = (int) (ads.computeVolts(ads.readADC_SingleEnded(1)) * 200.0F);
  voltage = ads.computeVolts(ads.readADC_SingleEnded(2)) * 1.5F;

  Serial.println("Soil Voltage: " + String(soilMv) + "mv");
  Serial.println("Lux: " + String(lux));
  Serial.println("Voltage: " + String(voltage, 2) + "v");
  Serial.println("Temp: " + String(insideTemp, 2) + "C");
  Serial.println("Hum: " + String(insideHum, 1) + "%");
  Serial.println("Pressure: " + String(insidePres) + "hPa");

  Serial.println("Sensor power off.");
  digitalWrite(SENSOR_PWR, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
}
