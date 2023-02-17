#include <AceButton.h>
#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

using namespace ace_button;
const int BUTTON_PIN = 14; //edit your switch pin here.
AceButton button(BUTTON_PIN);
void handleEvent(AceButton*, uint8_t, uint8_t);
int lock_state;

void setup() {
	Serial.begin(115200);
	wifi_connect(); // in wifi_info.h
	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	my_homekit_setup();
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.setEventHandler(handleEvent);
}

void loop() {
	my_homekit_loop();
  button.check();
	delay(10);
}

//==============================
// HomeKit setup and loop
//==============================

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on;
extern "C" homekit_characteristic_t cha_switch_on2;
extern "C" homekit_characteristic_t cha_switch_on3;
extern "C" homekit_characteristic_t cha_switch_on4;

static uint32_t next_heap_millis = 0;

#define PIN_SWITCH 2
#define PIN_SWITCH2 5
#define PIN_SWITCH3 0
#define PIN_SWITCH4 4
#define PIN_INPUT 14


//Called when the switch value is changed by iOS Home APP
void cha_switch_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_switch_on.value.bool_value = on;	//sync the value
	LOG_D("Switch: %s", on ? "ON" : "OFF");
  lock_state =  on ? 1 : 0;
	digitalWrite(PIN_SWITCH, on ? LOW : HIGH);
}
void cha_switch_on_setter2(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch_on2.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(PIN_SWITCH2, on ? LOW : HIGH);
}
void cha_switch_on_setter3(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch_on2.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(PIN_SWITCH3, on ? LOW : HIGH);
}
void cha_switch_on_setter4(const homekit_value_t value) {
  bool on = value.bool_value;
  cha_switch_on2.value.bool_value = on;  //sync the value
  LOG_D("Switch: %s", on ? "ON" : "OFF");
  digitalWrite(PIN_SWITCH4, on ? LOW : HIGH);
}

void my_homekit_setup() {
	pinMode(PIN_SWITCH, OUTPUT);
	digitalWrite(PIN_SWITCH, HIGH);
  pinMode(PIN_SWITCH2, OUTPUT);
  digitalWrite(PIN_SWITCH2, HIGH);
  pinMode(PIN_SWITCH3, OUTPUT);
  digitalWrite(PIN_SWITCH3, HIGH);
  pinMode(PIN_SWITCH4, OUTPUT);
  digitalWrite(PIN_SWITCH4, HIGH);

	//Add the .setter function to get the switch-event sent from iOS Home APP.
	//The .setter should be added before arduino_homekit_setup.
	//HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
	//Maybet his is a legacy design issue in the original esp-homekit library,
	//and I have no reason to modify this "feature".
	cha_switch_on.setter = cha_switch_on_setter;
  cha_switch_on2.setter = cha_switch_on_setter2;
   cha_switch_on3.setter = cha_switch_on_setter3;
    cha_switch_on4.setter = cha_switch_on_setter4;
	arduino_homekit_setup(&config);

	//report the switch value to HomeKit if it is changed (e.g. by a physical button)
	//bool switch_is_on = true/false;
	//cha_switch_on.value.bool_value = switch_is_on;
	//homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
}

void my_homekit_loop() {
	arduino_homekit_loop();

	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		LOG_D("Free heap: %d, HomeKit clients: %d",
				ESP.getFreeHeap(), arduino_homekit_connected_clients_count());
	}
}



void handleEvent(AceButton* /* button */, uint8_t eventType,
  uint8_t buttonState ) {

    switch (eventType) {
    case AceButton::kEventPressed:
      if(lock_state==0){
      cha_switch_on.value.bool_value = true;
      bool on = cha_switch_on.value.bool_value;
      Serial.println("LED:ON(Force) rapazzz");
      digitalWrite(PIN_SWITCH, on ? LOW : HIGH);
      homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
      lock_state = 1;
      }
      else {
      cha_switch_on.value.bool_value = false;
      bool on = cha_switch_on.value.bool_value;
      Serial.println("LED:OFF(Force) manoo");
      digitalWrite(PIN_SWITCH, on ? LOW : HIGH);
      homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
      lock_state = 0;
      }
      break;
    case AceButton::kEventReleased:
      if(lock_state==0){
      cha_switch_on.value.bool_value = true;
      bool on = cha_switch_on.value.bool_value;
      Serial.println("LED:ON(Force) rapazzz");
      digitalWrite(PIN_SWITCH, on ? LOW : HIGH);
      homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
      lock_state = 1;
      }
      else {
      cha_switch_on.value.bool_value = false;
      bool on = cha_switch_on.value.bool_value;
      Serial.println("LED:OFF(Force) manoo");
      digitalWrite(PIN_SWITCH, on ? LOW : HIGH);
      homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
      lock_state = 0;
      }
      break;
  }
}
