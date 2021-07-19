#include "_config.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>

#include <Wire.h>
#include <SI7021.h>

SI7021 sensor;

#ifdef  _COMPILE_BMP_280
  #include <Adafruit_BMP280.h>
  Adafruit_BMP280 bmp; // use I2C interface
  Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
  Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
#endif

WiFiClient wifiClient;
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;

String wifiMacString;
String wifiIPString;

// Sending interval for millis instead of delay
const int sendInterval = 30000;
unsigned long int t = 0;

void handleRoot() {
  char temp[1000];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  float t = (float) sensor.getCelsiusHundredths() / 100;
  int h = sensor.getHumidityPercent();
  snprintf(temp, 1000,
           "<html>\
  <head>\
    <meta http-equiv='refresh' content='30'/>\
    <title>Festoon Sensor Hub</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Festoon Leg 2 Top</h1>\
    <p>Temperature: %.2f&deg;C</p>\
    <p>Humidity: %2d&percnt;</p>\
    <p>Uptime: %02d:%02d:%02d</p>\
  </body>\
</html>", t, h, hr, min % 60, sec % 60
          );
  httpServer.send(200, "text/html", temp);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += httpServer.uri();
  message += "\nMethod: ";
  message += (httpServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += httpServer.args();
  message += "\n";

  for (uint8_t i = 0; i < httpServer.args(); i++) {
    message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
  }

  httpServer.send(404, "text/plain", message);
}

void setup()
{
    Serial.begin(115200);

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
    
    #ifdef _COMPILE_BMP_280
      bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID);
    #endif
    
    MDNS.begin(host);
  
    httpUpdater.setup(&httpServer, update_path, update_username, update_password);
    httpServer.on("/", handleRoot);
    httpServer.onNotFound(handleNotFound);
    httpServer.begin();
  
    MDNS.addService("http", "tcp", 80);
    Serial.printf("HTTPUpdateServer ready! Open http://%s.local%s in your browser and login with username '%s' and password '%s'\n", host, update_path, update_username, update_password);

}

void loop()
{
    httpServer.handleClient();
    MDNS.update();
    #ifdef _COMPILE_BMP_280
        sensors_event_t temp_event, pressure_event; 
        bmp_temp->getEvent(&temp_event);
        bmp_pressure->getEvent(&pressure_event);
    #endif
    
        //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        // Your Domain name with URL path or IP address with path
        http.begin(wifiClient, serverName);

        // Specify content-type header
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String httpRequestData = apiKeyName + "=" + apiKeyValue + "&sensor=" + sensorName + "&location=" + sensorLocation;
        httpRequestData = httpRequestData + "&temperature=" + sensor.getCelsiusHundredths() + "&humidity=" + sensor.getHumidityPercent();
        httpRequestData = httpRequestData + "&mac_address=" + String(wifiMacString) + "&ip_address=" + String(wifiIPString);
        
        #ifdef _COMPILE_BMP_280
          httpRequestData = httpRequestData + "&pressure=" + pressure_event.pressure + "&bmp_temp=" + temp_event.temperature;
        #endif
        
        httpRequestData = httpRequestData + "&sensor_id=" + sensorId + "";
        
        // uncomment to see the post data in the monitor
        if (millis() > (sendInterval + t)) {
          Serial.println(httpRequestData);
          digitalWrite(LED_BUILTIN, LOW);
          
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
          digitalWrite(LED_BUILTIN, HIGH);
          t = millis();
        }
        // Free resources
        http.end();
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }    
}
