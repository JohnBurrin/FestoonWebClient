#ifndef _CONFIG_H
#define _CONFIG_H
// Replace with your network credentials
const char *ssid = "BTHub5-PGFX_Private";
const char *password = "c@Z5wvM!Ut";

String sensorLocation = "Roaming";
String sensorId = "R1";
const char* host = "R1-festoon-sensor";
const char* update_path = "/firmware";
const char* update_username = "admin";
const char* update_password = "admin";

// REPLACE with your Domain name and URL path or IP address with path
const char *serverName = "http://app.jointine.uk/api/admin/festoon";

// // API Key should we need it.
String apiKeyName = "api_key";
String apiKeyValue = "tPmAT5Ab3j7F9";

String sensorName = "SI1702";

// remove this define to not compile BMP280 sensor
#define _COMPILE_BMP_280

#endif
