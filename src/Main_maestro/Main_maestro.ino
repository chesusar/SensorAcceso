#include <Arduino.h>

////Librerias dfplayer////
#include "Alarma.h"

////Librerias RFID////
#include "UID.h"

////Instancias dfplayer////
int activa = 0;

////Instancias RFID////
String UID = "";
String UID_valido = " f3 5a 4c 17";

////Instancias comunicacion////
int recibido = 0;

void setup(){
  Serial.begin(115200);

////Configurar dfplayer////
  Config_alarma();
}

void loop(){
  if(recibido == 1){
    if(activa == 0){
      Activar_alarma();
      activa = 1;
    }
    else if(activa == 1){
      Bucle_alarma();
    }

    UID = leer_UID();

    if(UID == UID_valido){        // Añadir "o tecleado contraseña correcta"
      Serial.println("Valido");
      activa = 0;
      recibido = 0;
      Desactivar_alarma();
    }

  }
}
