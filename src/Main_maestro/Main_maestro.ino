#include <Arduino.h>

////Librerias dfplayer////
#include "Alarma.h"

////Librerias RFID////
#include "UID.h"

////Librerias pantalla////
#include "Pantallas.h"

////Librerias keypad////
#include "Keypad.h"

////Librerias keypad////
#include "Comunicacion.h"

////Instancias dfplayer////
int activa = 0;

////Instancias RFID////
String UID = "";
String UID_valido = " f3 5a 4c 17";

////Instancias pantalla////
int frame = 0;

////Instancias keypad////
String pin = "";
String pin_correcto = "9876";

////Instancias comunicacion////
int recibido = 0;

////Instancias comunicacion////
int tiempo1 = 0;
int tiempo2 = 0;
int tiempo3 = 0;
int tiempo4 = 0;

////Estados////
int estado = 0;
const int ST_INICIO = 0;
const int ST_ESPERA = 1;
const int ST_PIN_ERROR = 2;
const int ST_ALERTA = 3;
const int ST_CLAVE = 4;
const int ST_PIN_ERROR_2 = 5;
const int ST_DESBLOQUEO = 6;
const int ST_DESACTIVADO = 7;
const int ST_ACTIVACION = 8;
const int ST_PIN_ERROR_3 = 9;
const int ST_ESPERA_2 = 10;

int tiempo5 = 0;  // borrar en la version final
int tiempo6 = 0;  // borrar en la version final

void setup() {
  Serial.begin(115200);

  ////Configuracion dfplayer////
  Config_alarma();

  ////Configuracion pantalla////
  Config_pantalla();

  ////Configuracion keypad////
  Config_keypad(columnPins, rowPins);

  ////Configuracion Comunicacion////
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("Iniciando wifi");
  Serial.println(WiFi.macAddress());
  communication_init(NULL, muerto);
  
  tiempo5 = millis();  // borrar en la version final
  
}

void loop() {

  if (estado == ST_INICIO) {
    pantalla_inicio();
    tiempo6 = millis();  // borrar en la version final

    if ((tiempo6 - tiempo5) > 10000) {  // cambiar por callback
      tiempo1 = millis();
      estado = ST_ESPERA;
    }
  }

  if (estado == ST_ESPERA) {
    pantalla_clave();

    char input = input_getChar();

    if (input == '*') {
      pin = "";
    }

    else if (input != KEY_NULL && pin.length() < 4) {
      pin += input;
    }

    else if (input == '#') {
      if (pin == pin_correcto) {
        pin = "";
        estado = ST_DESBLOQUEO;
      }

      else {
        pin = "";
        estado = ST_PIN_ERROR;
      }
    }

    UID = leer_UID();

    if (UID == UID_valido) {
      estado = ST_DESBLOQUEO;
      frame = 0;
    }

    display.setCursor(32, 32);  // Start at top-left corner
    display.print(pin[0]);
    display.setCursor(52, 32);  // Start at top-left corner
    display.print(pin[1]);
    display.setCursor(72, 32);  // Start at top-left corner
    display.print(pin[2]);
    display.setCursor(92, 32);  // Start at top-left corner
    display.print(pin[3]);

    display.display();

    tiempo2 = millis();

    if ((tiempo2 - tiempo1) > 30000) {
      pin = "";
      estado = ST_ALERTA;
    }
  }

  if (estado == ST_PIN_ERROR) {
    pantalla_pin_error();
    delay(1500);
    tiempo1 = millis();
    estado = ST_ESPERA;
  }

  if (estado == ST_ALERTA) {
    recibido = 0;

    display.clearDisplay();
    display.drawBitmap(32, 0, icono1[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT_icono1;
    delay(FRAME_DELAY);

    char input = input_getChar();

    if (input != KEY_NULL) {
      estado = ST_CLAVE;
      frame = 0;
    }

    UID = leer_UID();

    if (UID == UID_valido) {
      estado = ST_DESBLOQUEO;
      frame = 0;
    }

    if (activa == 0) {
      Activar_alarma();
      activa = 1;
    } else if (activa == 1) {
      Bucle_alarma();
    }
  }

  if (estado == ST_CLAVE) {
    pantalla_clave();

    char input = input_getChar();

    if (input == '*') {
      pin = "";
    }

    else if (input != KEY_NULL && pin.length() < 4) {
      pin += input;
    }

    else if (input == '#') {
      if (pin == pin_correcto) {
        pin = "";
        estado = ST_DESBLOQUEO;
      }

      else {
        pin = "";
        estado = ST_PIN_ERROR_2;
      }
    }

    UID = leer_UID();

    if (UID == UID_valido) {
      estado = ST_DESBLOQUEO;
      frame = 0;
    }

    display.setCursor(32, 32);  // Start at top-left corner
    display.print(pin[0]);
    display.setCursor(52, 32);  // Start at top-left corner
    display.print(pin[1]);
    display.setCursor(72, 32);  // Start at top-left corner
    display.print(pin[2]);
    display.setCursor(92, 32);  // Start at top-left corner
    display.print(pin[3]);

    display.display();
  }

  if (estado == ST_PIN_ERROR_2) {
    pantalla_pin_error();
    delay(1500);
    estado = ST_CLAVE;
  }

  if (estado == ST_DESBLOQUEO) {
    Desactivar_alarma();
    activa = 0;

    display.clearDisplay();
    display.drawBitmap(32, 0, icono2[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT_icono2;
    delay(FRAME_DELAY);
    if (frame == 27) {
      estado = ST_DESACTIVADO;
      frame = 0;
    }
  }

  if (estado == ST_DESACTIVADO) {
    pantalla_desactivada();

    char input = input_getChar();

    if (input != KEY_NULL) {
      estado = ST_ACTIVACION;
      pin = "";
    }

    UID = leer_UID();

    if (UID == UID_valido) {
      tiempo3 = millis();
      estado = ST_ESPERA_2;
    }
  }

  if (estado == ST_ACTIVACION) {
    pantalla_clave();

    char input = input_getChar();

    if (input == '*') {
      pin = "";
    }

    else if (input != KEY_NULL && pin.length() < 4) {
      pin += input;
    }

    else if (input == '#') {
      if (pin == pin_correcto) {
        pin = "";
        tiempo3 = millis();
        estado = ST_ESPERA_2;
      }

      else {
        pin = "";
        estado = ST_PIN_ERROR_3;
      }
    }

    UID = leer_UID();

    if (UID == UID_valido) {
      tiempo3 = millis();
      estado = ST_ESPERA_2;
    }

    display.setCursor(32, 32);  // Start at top-left corner
    display.print(pin[0]);
    display.setCursor(52, 32);  // Start at top-left corner
    display.print(pin[1]);
    display.setCursor(72, 32);  // Start at top-left corner
    display.print(pin[2]);
    display.setCursor(92, 32);  // Start at top-left corner
    display.print(pin[3]);

    display.display();
  }

  if (estado == ST_PIN_ERROR_3) {
    pantalla_pin_error();
    delay(1500);
    estado = ST_ACTIVACION;
  }

  if (estado == ST_ESPERA_2) {
    tiempo4 = millis();
    pantalla_activando();

    if ((tiempo4 - tiempo3) > 60000) {
      estado = ST_INICIO;
    }
  }
}

void muerto(){
}