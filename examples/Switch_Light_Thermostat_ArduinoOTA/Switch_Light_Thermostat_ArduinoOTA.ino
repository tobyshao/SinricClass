#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#include <Sinric.h>

#include <SinricSwitch.h>
#include <SinricLight.h>
#include <SinricThermostat.h>

// To Do: Put in your credentials

const char* ssid = "WIFI-SSID";
const char* pass = "WIFI-PASSWORD";

const char* api_key = "1aaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";

const char* switchId = "500000000000000000000000";
const char* lightId  = "511111111111111111111111";
const char* thermoId = "522222222222222222222222";

// callbacks for devices...

void onPowerState(SinricDevice& device, bool state)          { Serial.printf("onPowerState(%s,%s)\r\n", device.getDeviceId(), state?"on":"off"); }

void onSetColor(SinricLight& device, double hue, double sat, double val) { Serial.printf("onSetColor(%s,%f,%f,%f)\r\n", device.getDeviceId(), hue, sat, val); }
void onSetBrightness(SinricLight& device, int bri)                       { Serial.printf("onSetBrightness(%s,%d)\r\n", device.getDeviceId(), bri); }
void onAdjustBrightness(SinricLight& device, int bri)                    { Serial.printf("onAdjustBrightness(%s,%d)\r\n", device.getDeviceId(), bri); }
void onDecColorTemperature(SinricLight& device)                          { Serial.printf("onDecColorTemperature(%s)\r\n", device.getDeviceId()); }
void onIncColorTemperature(SinricLight& device)                          { Serial.printf("onIncColorTemperature(%s)\r\n", device.getDeviceId()); }
void onSetColorTemperature(SinricLight& device, int color_temp)          { Serial.printf("onSetColorTemperature(%s,%d)\r\n", device.getDeviceId(), color_temp); }

void onSetTargetTemperature(SinricThermostat& device, double temp, const String& scale)    { Serial.printf("onSetTargetTemperature(%s,%f,%s)\r\n", device.getDeviceId(), temp, scale.c_str()); }
void onAdjustTargetTemperature(SinricThermostat& device, double temp, const String& scale) { Serial.printf("onAdjustTargetTemperature(%s,%f,%s)\r\n", device.getDeviceId(), temp, scale.c_str()); }
void onSetThermostatMode(SinricThermostat& device, const String& mode)                     { Serial.printf("onSetThermostatMode(%s,%s)\r\n", device.getDeviceId(), mode.c_str()); }

// WiFi setup
void setupWifi() {
  Serial.begin(115200);
  delay(1000);
  Serial.printf("\r\nConnecting WiFi (%s)", ssid);

  WiFi.persistent(false);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("connected!\r\n");

  IPAddress localIP = WiFi.localIP();
  Serial.printf("IP address: %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

// Sinric setup
void setupSinric() {
  Serial.print("Connecting Sinric");
  Sinric.begin(api_key);
  while (!Sinric.isConnected()) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("connected!");

  // add new Switch
  SinricSwitch& mySwitch = Sinric.add<SinricSwitch>(switchId);
  // set the callback
  mySwitch.onPowerState(onPowerState);
  //mySwitch.onPowerState([](const char* deviceId, bool state) { Serial.printf("lambda power state %s,%s\r\n", deviceId, state?"true":"false"); } );

  // add a new Light
  SinricLight& myLight = Sinric.add<SinricLight>(lightId);
  // set the callbacks
  myLight.onPowerState(onPowerState);
  myLight.onSetColor(onSetColor);
  myLight.onSetBrightness(onSetBrightness);
  myLight.onAdjustBrightness(onAdjustBrightness);
  myLight.onIncColorTemperature(onIncColorTemperature);
  myLight.onDecColorTemperature(onDecColorTemperature);
  myLight.onSetColorTemperature(onSetColorTemperature);

  // add new Thermostat
  SinricThermostat& myThermo = Sinric.add<SinricThermostat>(thermoId);
  // set the callbacks for thermostat
  myThermo.onPowerState(onPowerState);
  myThermo.onSetTargetTemperature(onSetTargetTemperature);
  myThermo.onAdjustTargetTemperature(onAdjustTargetTemperature);
  myThermo.onSetThermostatMode(onSetThermostatMode);
}

// ArudinoOTA setup
void setupOTA() {
  ArduinoOTA.onStart([]() { Serial.printf("Start updating %s\r\n", ArduinoOTA.getCommand() == U_FLASH ? "sketch" : "spiffs"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void setup() {
  delay(1000);
  Serial.begin(115200); Serial.println();
  setupWifi();
  setupOTA();
  setupSinric();
}

void loop() {
  ArduinoOTA.handle();
  Sinric.handle();
}
