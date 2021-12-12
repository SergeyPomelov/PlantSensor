#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <WiFiFunctions.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

HTTPClient http;
WiFiClient wifi;

void WiFiinit()
{
  Serial.print(F("Initialising Wifi "));
  WiFi.mode(WIFI_STA);
  WiFi.config(IP, GATE, MASK);
  WiFi.begin(AP_SSID, AP_PASS);
  WiFi.persistent(true);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
}

void hibernate()
{
  ESP.deepSleep(SLEEP_SEC * 1000U * 1000U);
  delay(10);
}

void wait()
{
  delay(SLEEP_SEC * 1000U);
}

boolean WiFiconnect()
{
  if (WiFi.status() != WL_CONNECTED && WiFi.SSID() != AP_SSID)
  {
    WiFiinit();
  }

  unsigned long startTime = millis();
  unsigned long tries = 1;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print('.');
    unsigned long elapsedTime = millis() - startTime;
    if (elapsedTime > 15000)
    {
      WiFiinit();
      startTime = millis();
      tries++;
    }
    if (tries > 3)
    {
      hibernate();
    }
  }
  Serial.print(" Connected! IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

int updateDevice(const uint16_t idx, const String value, const boolean terminal)
{

  if (value == String("nan")) {
    Serial.println("Won't send nan");
    return 0;
  }

  int httpCode = 0;
  String url = "http://" + String(DOMOTICZ_IP) + ":" + DOMOTICZ_PORT +
               "/json.htm?type=command&param=udevice&idx=" + String(idx) +
               "&nvalue=" + value + "&svalue=" + value;

  http.begin(wifi, url);
  http.addHeader("Accept", "*/*");
  http.addHeader("Host", DOMOTICZ_IP);
  http.addHeader("Upgrade-Insecure-Requests", "1");
  http.addHeader("User-Agent", "ESP 12-E v2 Plant Sensor");

  if (terminal)
  {
    http.addHeader("Connection", "close");
  }

  Serial.println((char *)url.c_str());
  httpCode = http.GET();

  if (httpCode > 0)
  {
    if (httpCode != HTTP_CODE_OK)
    {
      String payload = http.getString();
      Serial.println(payload);
    }
  }
  else
  {
    Serial.printf("Domoticz send failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  return httpCode;
}

void sendDataDomoticz() {
  digitalWrite(LED_BUILTIN, LOW);
  if (WiFiconnect())
  {
    digitalWrite(LED_BUILTIN, HIGH);
    updateDevice(213U, String(insideTemp), false);
    digitalWrite(LED_BUILTIN, LOW);
    updateDevice(214U, String(insideHum), false);
    digitalWrite(LED_BUILTIN, HIGH);
    updateDevice(211U, String(soilMv), false);
    digitalWrite(LED_BUILTIN, LOW);
    updateDevice(215U, String(voltage, 2), false);
    digitalWrite(LED_BUILTIN, HIGH);
    updateDevice(212U, String(lux), true);
    digitalWrite(LED_BUILTIN, LOW);
  }
  digitalWrite(LED_BUILTIN, HIGH);
}
