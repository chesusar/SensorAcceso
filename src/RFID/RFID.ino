#include <SPI.h>
#include <MFRC522.h>
#include "UID.h"


void setup() {
  Serial.begin(115200);
  Serial.println("Acerca la tarjeta");
}

void loop() {
  leer_UID();
  delay(500);
}