# SinricClass

An easy to use class to connect to io.sinric.com and make your ESP-Projects alexa compatible
- Big thank's to [kakopappa](https://github.com/kakopappa/sinric) for inventing and hosting sinric.com!

## usage
#### include Sinric-Library (Sinric.h) and Sinric-Device-Libraries (eg. SinricSwitch.h, SinricLight.h ...) an define credentials

```
...
#include <Sinric.h>
#include <SinrichSwitch.h>
...
const char* api_key = "your-api-key"
const char* deviceId = "your-device-id"
...
```
#### define callback routine(s)
```
void onPowerState(const string& deviceId, bool state) {
  Serial.printf("device %s turned %s\r\n, deviceId.c_str(), state?"on":"off");
}
```
#### In setup()
```
  ...
  Sinric.begin(api_key);
  // add a switch
  SinricSwitch& mySwitch Sinric.add<SinricSwitch>(deviceId);
  // set callback function
  mySwitch.onPowerState(onPowerState);
  ...
```
  
#### In loop()
```
  ...
  Sinric.handle();
  ...
```
## Implemented devices
- Switch (SinricSwitch in SinricSwitch.h)
- Light (SinricLight in SinricLight.h)
- Thermostat (SinricThermostat in SinricThermostat.h)

## Simple add new or not yet implemented devies
- Make a new class derivered from SinricDevice (defined in SinricDevice.h)
- Override constructor for propper callback init
- Define prototype functions for needed callbacks
- Provide callback setter function
- Override handle() to handle incomming commands and call the right callbacks

- Use the new device like the pre-defined 
```
myNewSinricDevice& newDevice = Sinric.add<myNewSinricDevice>(deviceId);
newDevice.CallbackSetFunction(callback-function);
```

## Dependencies
- [WebSocketsClient](https://github.com/Links2004/arduinoWebSockets/releases)
- [ArduinoJson](https://arduinojson.org/)
