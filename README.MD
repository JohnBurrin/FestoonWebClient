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
