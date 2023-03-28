#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.
void setup() {
  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266
}

void loop() {
  Serial.println("NEC");
  irsend.sendNEC(0xF7C03F);//shutdown the seewo computer
  Serial.println("Ligar ligar"); 
  irsend.sendNEC(0xF740BF); // Envie o código infravermelho
  Serial.println("Desligar"); 
  irsend.sendNEC(0xF7C03F); // Envie o código infravermelho
  Serial.println("Ligar ligar"); 
  irsend.sendNEC(0xF7609F); // Envie o código infravermelho
    delay(5000);
  irsend.sendNEC(0xF7A05F); // Envie o código infravermelho
    delay(5000);

  irsend.sendNEC(0xF720DF); // Envie o código infravermelho
    delay(5000);
  irsend.sendNEC(0xF7A857); // Envie o código infravermelho
  Serial.println("Cores..."); 
  delay(5000);

}
