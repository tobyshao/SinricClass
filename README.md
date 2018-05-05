# SinricClass

### usage
#### include an define credentials

```
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
