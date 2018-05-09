#ifndef sinricswitch_h_
#define sinricswitch_h_

#include <ArduinoJson.h>
#include "SinricDevice.h"

class SinricSwitch : public SinricDevice {
public:
	typedef std::function<void(SinricSwitch&, bool)> PowerStateCallback;

	SinricSwitch(const char* deviceId );

  void onPowerState(PowerStateCallback callback) { powerState = callback; }
	const char* test() { return deviceId; }

  void handle(const String& action, JsonObject& json) override;
private:
  PowerStateCallback powerState;
};

SinricSwitch::SinricSwitch(const char* deviceId ) :
  SinricDevice(deviceId),
  powerState(nullptr) {
}

void SinricSwitch::handle(const String& action, JsonObject& json) {
  // powerState
  if ((action == "setPowerState") && (powerState)) {
    bool state = (json["value"] == "ON");
    powerState(*this, state);
  }
}

#endif
