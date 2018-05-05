# SinricClass

An easy to use class to connect to io.sinric.com and make your ESP-Projects alexa compatible
- Big thank's to [kakopappa](https://github.com/kakopappa/sinric) for inventing and hosting sinric.com!

## usage
#### include Sinric-Library (Sinric.h) and Sinric-Device-Libraries (eg. SinricSwitch.h, SinricLight.h ...) an define credentials

```
...
#include <Sinric.h>
#include <SinrichSwitch>
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

### Dependencies
- [WebSocketsClient](https://github.com/Links2004/arduinoWebSockets/releases)
- [ArduinoJson](https://arduinojson.org/)
