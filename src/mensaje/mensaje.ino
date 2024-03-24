#include "Comunicacion.h"

uint8_t slaveAddress[] = {0x24, 0x0A, 0xC4, 0xC5, 0xDD, 0x48};  // alarma COM 11



void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_MODE_STA);
  Serial.println("Iniciando wifi");
  Serial.println(WiFi.macAddress());

  communication_init()

  //communication_init(NULL);         // Alarma COM 11
  communication_init(slaveAddress); // Sensores COM 14
}

void loop() {
  if (!communication_isHost()){
    return;
  }

  char buff[20];
  communication_send(slaveAddress, 1, itoa(random(0, 1000), buff, 10));
  delay(1000);

}
