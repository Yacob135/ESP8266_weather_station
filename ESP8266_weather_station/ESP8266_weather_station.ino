#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

const char* ssid = "your_ssid_here";
const char* password = "your_pass_here";

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

void setup () {

  Serial.begin(115200);

  Wire.begin(4, 5);
  bme.begin(0x76);

  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1,
                  Adafruit_BME280::SAMPLING_X1,
                  Adafruit_BME280::SAMPLING_X1,
                  Adafruit_BME280::FILTER_OFF);

  DS18B20.begin();

  uint16_t wifiTime = 0;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(10);
    wifiTime++;
    if (wifiTime > 1000) {
      break; //breaks the loop after 10 seconds of unsuccessful connection
    }
  }

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String stringVal;
    String httpReq;
    String payload;
    int httpCode;


    DS18B20.requestTemperatures();
    float temp = DS18B20.getTempCByIndex(0);

    if (temp > -80.0 && temp < 80.0) {
      stringVal = String(temp, 3);
      httpReq = String("http://your_server_here:8080/your_auth_token_here/update/V1?value=" + stringVal);
      http.begin(httpReq);
      httpCode = http.GET();
    }

    float bmeTemperature;
    float bmePressure;
    float bmeHumidity;

    bme.takeForcedMeasurement();
    bmeTemperature = bme.readTemperature();
    bmeHumidity = bme.readHumidity();
    bmePressure = bme.readPressure() / 100.0F;

    stringVal = String(bmeTemperature, 3);
    httpReq = String("http://your_server_here:8080/your_auth_token_here/update/V2?value=" + stringVal);
    http.begin(httpReq);  //Specify request destination
    httpCode = http.GET();

    stringVal = String(bmeHumidity, 3);
    httpReq = String("http://your_server_here:8080/your_auth_token_here/update/V3?value=" + stringVal);
    http.begin(httpReq);  //Specify request destination
    httpCode = http.GET();

    stringVal = String(bmePressure, 3);
    httpReq = String("http://your_server_here:8080/your_auth_token_here/update/V4?value=" + stringVal);
    http.begin(httpReq);  //Specify request destination
    httpCode = http.GET();

    float batteryVoltage = analogRead(A0) / 1024.0 * 4.4;
    stringVal = String(batteryVoltage, 2);
    httpReq = String("http://your_server_here:8080/your_auth_token_here/update/V5?value=" + stringVal);
    http.begin(httpReq);  //Specify request destination
    httpCode = http.GET();

    int16_t batteryPercentage = 389.344 - (288.378 * batteryVoltage) + (53.4812 * batteryVoltage * batteryVoltage);
    if (batteryPercentage >= 100) {
      batteryPercentage = 100;
    }
    if (batteryPercentage <= 0) {
      batteryPercentage = 0;
    }
    stringVal = batteryPercentage;
    httpReq = String("http://your_server_here:8080/your_auth_token_here/update/V6?value=" + stringVal);
    http.begin(httpReq);  //Specify request destination
    httpCode = http.GET();

    stringVal = String(millis());
    httpReq = String("http://your_server_here:8080/your_auth_token_here/update/V7?value=" + stringVal);
    http.begin(httpReq);  //Specify request destination
    httpCode = http.GET();

    http.end();   //Close connection
  }

  ESP.deepSleep(5 * 60e6);
}
