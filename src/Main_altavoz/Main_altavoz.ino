#include <Arduino.h>

////Librerias dfplayer////
#include "Alarma.h"

////Instancias dfplayer////
int activa = 0;

int alerta = 4;

void setup() {
  Serial.begin(115200);

  Serial.println("1");
  pinMode(alerta, INPUT);

  ////Configuracion dfplayer////
  Config_alarma();
  Serial.println("2"); 
}

void loop() {

  if (digitalRead(alerta) == HIGH){
    if (activa == 0) {
      Activar_alarma();
      activa = 1;
      Serial.println("activa");

    }
    else if (activa == 1) {
      Bucle_alarma();
      Serial.println("bucle"); 
    }
  }

  if (digitalRead(alerta) == LOW){
    Desactivar_alarma();
    activa = 0;
    Serial.println("desactivada"); 
  }
}

    