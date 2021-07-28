#Sensor Client

##Introduction
This application utilises the SI1702 and a BMP280 sensor on a NodeMCU v3 development board.


##Connections

|-----|-----|
9v ---> Vin 
0v ---> gnd

Nodemcu ----> SI7021 ---> BMP280
D1 / SCL --->   SCL  ---> SCL
D2 / SDA --->   SDA  ---> SDA
3v3 -------->  Vcc   ---> Vcc
Gnd -------->  Gnd   ---> Gnd


### Configuration
Mandatory
- copy config.example to config.h
- change ssid to your wifi ssid
- change password to your wifi password
- change serverName to the endpoint to recieve to posted data,

### Optional
- apiKeyValue can be set if your endpoint needs validation
- apiKeyName, this is the name of the key the enpoint is expecting to contain the api key
- sensorName, just a name to identifiy the sensor
- sensorLocation, a simple location description
- sensorId, and id for the sensor

### Other options
It is possiple to compile this WITHOUT a BMP280, if you want to do this, comment out the line
```#define _COMPILE_BMP_280```

## Config
There are many configuration options.

To set the wifi access set these to your wifi values
`const char *ssid = "XXXXXXXX";`
`const char *password = "XXXXX";`

These are for the web services
`const char* host = "S4-festoon-sensor"; // Use this to view the data in real time http://S4-festoon-sensor.local`
`const char* update_path = "/firmware";  // Use this to upload a new sketch update http://S4-festoon-sensor.local/firmware`
`const char* update_username = "admin";  // These are the creditials to access the firmware`
`const char* update_password = "admin";`


The device is designed to use a GET request to send the captured data to an API endpoint
`const char *serverName = "http://some-domain.co.uk";`

`String apiKeyName = "api_key"; // API Key should we need it.`
`String apiKeyValue = "please-changeme-if-you-need-me";`

so the above will add to the request 
`http://some-domain.co.uk?api_key=please-changeme-if-you-need-me`

A full request will look something like this 
`http://some-domain.co.uk?api_key=please-changeme-if-you-need-me@humidity=98&temperature=28.8&pressure=1024&sensor_id=S1`
