#include <Arduino.h>

////Librerias dfplayer////
#include "Alarma.h"

////Librerias RFID////
#include "UID.h"

////Librerias pantalla////
#include "Pantallas.h"

////Librerias keypad////
#include "Keypad.h"

////Instancias dfplayer////
int activa = 0;

////Instancias RFID////
String UID = "";
String UID_valido = " f3 5a 4c 17";

////Instancias pantalla////
int frame = 0;

////Instancias keypad////
String pin = "" ;
String pin_correcto = "9876";

////Instancias comunicacion////
int recibido = 0;

////Estados////
int estado = 0;
int inicio = 0;
int alerta = 1;
int clave = 2;
int pin_error = 3;
int desbloqueo = 4;

int tiempo1 = 0;  // borrar en la version final
int tiempo2 = 0;  // borrar en la version final

void setup(){
  Serial.begin(115200);

  ////Configuracion dfplayer////
  Config_alarma();

  ////Configuracion pantalla////
  Config_pantalla();

  ////Configuracion keypad////
  Config_keypad(columnPins, rowPins);

  tiempo1 = millis(); // borrar en la version final
}

void loop(){

  if(estado == inicio){
    pantalla_inicio();
    tiempo2 = millis(); // borrar en la version final

    
    if((tiempo2 - tiempo1) > 10000){  // poner recibir mensaje alerta // quitar calculo de tiempo
      recibido = 1;
    }
    
    if(recibido == 1){  // plantearse quitarlo
     estado = alerta;
    }
  }

  if(estado == alerta){
    recibido = 0;

    display.clearDisplay();
    display.drawBitmap(32, 0, icono1[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT_icono1;
    delay(FRAME_DELAY); 

    char input = input_getChar();

    if(input != KEY_NULL){
      estado = clave;
      pin = "";
      frame = 0;
    }

    UID = leer_UID();

    if(UID == UID_valido){
      estado = desbloqueo;
      frame = 0;
    }
    
    if(activa == 0){
      Activar_alarma();
      activa = 1;
    }
    else if(activa == 1){
      Bucle_alarma();
    }
  }

  if(estado == clave){
    pantalla_clave();

    char input = input_getChar();
    
    if(input == '*'){
      pin = "";
    }

    else if(input != KEY_NULL && pin.length() < 4){
      pin += input;
    }

    else if(input == '#'){
      if(pin == pin_correcto){
        estado = desbloqueo;
      }

      else{
        estado = pin_error;
      }
    }

    display.setCursor(32, 18);     // Start at top-left corner
    display.print(pin[0]);
    display.setCursor(52, 18);     // Start at top-left corner
    display.print(pin[1]);
    display.setCursor(72, 18);     // Start at top-left corner
    display.print(pin[2]);
    display.setCursor(92, 18);     // Start at top-left corner
    display.print(pin[3]);

    display.display();
  } 

  if(estado == pin_error){
    pantalla_pin_error();
    delay(1500);
    estado = clave;
  }

  if(estado == desbloqueo){
    Desactivar_alarma();
    activa = 0;

    display.clearDisplay();
    display.drawBitmap(32, 0, icono2[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT_icono2;
    delay(FRAME_DELAY);
    if(frame == 27){
      estado = inicio;
      frame = 0;
    }
  }
}
