#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <Wire.h>
#include <SI7021.h>
#include <Adafruit_BMP280.h>

#include "config.h"

//#define SDA D1 // GPIO0 on ESP-01 module
//#define SCL D2 // GPIO2 on ESP-01 module

SI7021 sensor;

Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

WiFiClient wifiClient;

String wifiMacString;
String wifiIPString;

void setup()
{
    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(ssid, password);

    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    wifiMacString = WiFi.macAddress();
    wifiIPString = WiFi.localIP().toString();

    sensor.begin(SDA, SCL);
    bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
}

void loop()
{
    sensors_event_t temp_event, pressure_event;
    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);
    
        //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(wifiClient, serverName);

        // Specify content-type header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String httpRequestData = "api_key=" + apiKeyValue + "&sensor=" + sensorName + "&location=" + sensorLocation;
        httpRequestData = httpRequestData + "&temperature=" + sensor.getCelsiusHundredths() + "&humidity=" + sensor.getHumidityPercent();
        httpRequestData = httpRequestData + "&mac_address=" + String(wifiMacString) + "&ip_address=" + String(wifiIPString);
        httpRequestData = httpRequestData + "&pressure=" + pressure_event.pressure + "&bmp_temp=" + temp_event.temperature;
        httpRequestData = httpRequestData + "&sensor_id=" + sensorId + "";
        
        // uncomment to see the post data in the monitor
        Serial.println(httpRequestData);
        
        // Send HTTP POST request
        int httpResponseCode = http.POST(httpRequestData);
        if (httpResponseCode > 0)
        {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
        }
        else
        {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }
        // Free resources
        http.end();
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }
    //Send an HTTP POST request every 60 seconds
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30000);
    digitalWrite(LED_BUILTIN, LOW);
}
