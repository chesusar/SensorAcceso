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
int inicio = 0;
int espera1 = 1;
int pin_error1 = 2;
int alerta = 3;
int clave = 4;
int pin_error2 = 5;
int desbloqueo = 6;
int desactivado = 7;
int activacion = 8;
int pin_error3 = 9;
int espera2 = 10;

const int ST_INICIO = 0;
const int ST_ESPERA = 1;
const int ST_PIN_ERROR_1 = 2;
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
  communication_init(NULL);


  tiempo5 = millis();  // borrar en la version final
}

void loop() {

  if (estado == inicio) {
    pantalla_inicio();
    tiempo6 = millis();  // borrar en la version final


    if ((tiempo6 - tiempo5) > 10000) {  // poner recibir mensaje alerta // quitar calculo de tiempo
      recibido = 1;
    }

    if (recibido == 1) {  // plantearse quitarlo
      tiempo1 = millis();
      estado = espera1;
    }
  }

  if (estado == espera1) {
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
        estado = desbloqueo;
      }

      else {
        pin = "";
        estado = pin_error1;
      }
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
      estado = alerta;
    }
  }

  if (estado == pin_error1) {
    pantalla_pin_error();
    delay(1500);
    estado = espera1;
  }

  if (estado == alerta) {
    recibido = 0;

    display.clearDisplay();
    display.drawBitmap(32, 0, icono1[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT_icono1;
    delay(FRAME_DELAY);

    char input = input_getChar();

    if (input != KEY_NULL) {
      estado = clave;
      frame = 0;
    }

    UID = leer_UID();

    if (UID == UID_valido) {
      estado = desbloqueo;
      frame = 0;
    }

    if (activa == 0) {
      Activar_alarma();
      activa = 1;
    } else if (activa == 1) {
      Bucle_alarma();
    }
  }

  if (estado == clave) {
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
        estado = desbloqueo;
      }

      else {
        pin = "";
        estado = pin_error2;
      }
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

  if (estado == pin_error2) {
    pantalla_pin_error();
    delay(1500);
    estado = clave;
  }

  if (estado == desbloqueo) {
    Desactivar_alarma();
    activa = 0;

    display.clearDisplay();
    display.drawBitmap(32, 0, icono2[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT_icono2;
    delay(FRAME_DELAY);
    if (frame == 27) {
      estado = desactivado;
      frame = 0;
    }
  }

  if (estado == desactivado) {
    pantalla_desactivada();

    char input = input_getChar();

    if (input != KEY_NULL) {
      estado = activacion;
      pin = "";
    }

    UID = leer_UID();

    if (UID == UID_valido) {
      estado = espera2;
    }
  }

  if (estado == activacion) {
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
        estado = espera2;
      }

      else {
        pin = "";
        estado = pin_error3;
      }
    }

    display.setCursor(32, 18);  // Start at top-left corner
    display.print(pin[0]);
    display.setCursor(52, 18);  // Start at top-left corner
    display.print(pin[1]);
    display.setCursor(72, 18);  // Start at top-left corner
    display.print(pin[2]);
    display.setCursor(92, 18);  // Start at top-left corner
    display.print(pin[3]);

    display.display();
  }

  if (estado == pin_error3) {
    pantalla_pin_error();
    delay(1500);
    estado = activacion;
  }

  if (estado == espera2) {
    tiempo6 = millis();
    pantalla_activando();

    if ((tiempo4 - tiempo3) > 60000) {
      estado = inicio;
    }
  }
}
