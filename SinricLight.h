#ifndef sinriclight_h_
#define sinriclight_h_

#include <ArduinoJson.h>
#include "SinricDevice.h"

class SinricLight : public SinricDevice {
public:
  SinricLight(const char* deviceId);

  typedef std::function<void(SinricDevice&, bool)>                  PowerStateCallback;
  typedef std::function<void(SinricLight&, int)>                    SetBrightnessCallback;
  typedef std::function<void(SinricLight&, int)>                    AdjustBrightnessCallback;
  typedef std::function<void(SinricLight&, double, double, double)> SetColorCallback;
  typedef std::function<void(SinricLight&)>                         DecColorTemperatureCallback;
  typedef std::function<void(SinricLight&)>                         IncColorTemperatureCallback;
  typedef std::function<void(SinricLight&, int)>                    SetColorTemperatureCallback;

  void onPowerState(PowerStateCallback callback)                   { powerState = callback; }
  void onSetBrightness(SetBrightnessCallback callback)             { setBrightness = callback; }
  void onAdjustBrightness(AdjustBrightnessCallback callback)       { adjustBrightness = callback; }
  void onSetColor(SetColorCallback callback)                       { setColor = callback; }
  void onDecColorTemperature(DecColorTemperatureCallback callback) { decColorTemperature = callback; }
  void onIncColorTemperature(IncColorTemperatureCallback callback) { decColorTemperature = callback; }
  void onSetColorTemperature(SetColorTemperatureCallback callback) { setColorTemperature = callback; }

  void handle(const String& action, JsonObject& json) override;
private:
  PowerStateCallback          powerState;
  SetBrightnessCallback       setBrightness;
  AdjustBrightnessCallback    adjustBrightness;
  SetColorCallback            setColor;
  DecColorTemperatureCallback decColorTemperature;
  IncColorTemperatureCallback incColorTemperature;
  SetColorTemperatureCallback setColorTemperature;
};

SinricLight::SinricLight(const char* deviceId) :
  SinricDevice(deviceId),
  powerState(nullptr),
  setBrightness(nullptr),
  adjustBrightness(nullptr),
  setColor(nullptr),
  decColorTemperature(nullptr),
  incColorTemperature(nullptr),
  setColorTemperature(nullptr) {
};


void SinricLight::handle(const String& action, JsonObject& json) {
  // setPowerState
  if ((action == "setPowerState") && (powerState)) {
    bool state = (strcmp("ON", json["value"]) == 0);
    powerState(*this, state);
    // setColor
  } else if ((action == "SetColor") && (setColor)) {
    double hue = json["value"]["hue"];
    double sat = json["value"]["saturation"];
    double bri = json["value"]["brightness"];
    setColor(*this, hue, sat, bri);

    // setBrightness
  } else if ((action == "SetBrightness") && (setBrightness)) {
    int brightness = json["value"];
    setBrightness(*this, brightness);

    // AdjustBrightness
  } else if ((action == "AdjustBrightness") && (adjustBrightness)) {
    int brightness_adj = json["value"];
    adjustBrightness(*this, brightness_adj);

    // DecreaseColorTemperature
  } else if ((action == "DecreaseColorTemperature") && (decColorTemperature)) {
    decColorTemperature(*this);

    // IncreaseColorTemperature
  } else if ((action == "IncreaseColorTemperature") && (incColorTemperature)) {
    incColorTemperature(*this);

    // SetColorTemperature
  } else if ((action == "SetColorTemperature") && (setColorTemperature)) {
    int colorTemp = json["value"];
    setColorTemperature(*this, colorTemp);
  }
}

#endif
