#include <Arduino.h>

////Librerias dfplayer////
#include "Alarma.h"

////Instancias dfplayer////
int activa = 0;

void setup(){
  Serial.begin(115200);

////Configurar dfplayer////
  Config_alarma();
}

void loop(){
  if(activa == 0){
    Activar_alarma();
    activa = 1;
  }

  else if(activa == 1){
    Bucle_alarma();
  }

  delay(10000);
  Desactivar_alarma();
  activa = 0;
  delay(10000);
}
