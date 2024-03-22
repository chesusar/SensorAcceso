#include <Arduino.h>

////Librerias RFID////
#include "UID.h"

////Instancias RFID////
String UID = "";
String UID_valido = " f3 5a 4c 17";

void setup() {
  Serial.begin(115200);
  Serial.println("Acerca la tarjeta");
}

void loop() {
  UID = leer_UID();
  Serial.println(UID);
  
  if(UID == UID_valido){
    Serial.println("Valido");
  }
  delay(500);
}