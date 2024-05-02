#include <Arduino.h>

////Librerias RFID////
#include "UID.h"

////Librerias pantalla////
#include "Pantallas.h"

////Librerias keypad////
#include "Keypad.h"

////Librerias comunicacion////
#include "Comunicacion.h"

////Librerias MQTT////
#include "MQTT.h"

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
int tiempo1 = 0;
int tiempo2 = 0;
int tiempo3 = 0;
int tiempo4 = 0;

////Instancias MQTT////
String receivedValue = "5";
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
EspMQTTClient client(WIFI_SSID, WIFI_PASS, "io.adafruit.com", IO_USERNAME, IO_KEY, "TestClient", 1883);

////Instancias DFplayer////
int altavoz = 15;

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
  Serial.println("1");
  ////Configuracion pantalla////
  Config_pantalla();
  Serial.println("2");
  ////Configuracion keypad////
  Config_keypad(columnPins, rowPins);
  Serial.println("3");
  ////Configuracion Comunicacion////
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("Iniciando wifi");
  Serial.println(WiFi.macAddress());
  communication_init(NULL, muerto);

  ////MQTT////
  Serial.print("Conectando a Adafruit IO");
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Conectado a Adafruit IO");
  client.enableDebuggingMessages();
  client.loop();

  ////DFplayer////
  pinMode(altavoz, OUTPUT);

  tiempo5 = millis();  // borrar en la version final
  
}

void loop() {
  io.run();

  if (estado == ST_INICIO) {
    pantalla_inicio();

    io.feed("estado")->save(0);

    tiempo6 = millis();  // borrar en la version final

    if ((tiempo6 - tiempo5) > 10000) {  // cambiar por callback
      tiempo1 = millis();
      estado = ST_ESPERA;
    }

    if (receivedValue == "1"){
      receivedValue = "5";
      estado = ST_ALERTA;
    }
  }
  
  if (estado == ST_ESPERA) {
    pantalla_clave();

    io.feed("estado")->save(1);

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

    if (UID == UID_valido || receivedValue == "0") {
      receivedValue = "5";
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

    io.feed("estado")->save(2);

    delay(1500);
    tiempo1 = millis();
    estado = ST_ESPERA;
  }

  if (estado == ST_ALERTA) {
    recibido = 0;

    io.feed("alarma")->save(1);
    digitalWrite(altavoz, HIGH);

    io.feed("estado")->save(3);

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

    if (UID == UID_valido || receivedValue == "0") {
      receivedValue = "5";
      estado = ST_DESBLOQUEO;
      frame = 0;
    }
  }

  if (estado == ST_CLAVE) {
    pantalla_clave();

    io.feed("estado")->save(4);

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

    if (UID == UID_valido || receivedValue == "0") {
      receivedValue = "5";
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

    io.feed("estado")->save(5);

    delay(1500);
    estado = ST_CLAVE;
  }

  if (estado == ST_DESBLOQUEO) {
    io.feed("alarma")->save(0);
    digitalWrite(altavoz, LOW);

    io.feed("estado")->save(6);

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

    io.feed("estado")->save(7);

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

    if (receivedValue == "1"){
      receivedValue = "5";
      estado = ST_ALERTA;
    }
  }

  if (estado == ST_ACTIVACION) {
    pantalla_clave();

    io.feed("estado")->save(8);

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

    if (receivedValue == "1"){
      receivedValue = "5";
      estado = ST_ALERTA;
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

    io.feed("estado")->save(9);

    delay(1500);
    estado = ST_ACTIVACION;
  }

  if (estado == ST_ESPERA_2) {
    tiempo4 = millis();

    pantalla_activando();

    io.feed("estado")->save(10);

    if ((tiempo4 - tiempo3) > 60000) {
      estado = ST_INICIO;
    }
  }
}

void muerto(){
}

void onConnectionEstablished() {
  Serial.println("Conexión establecida");
  client.subscribe(FEED, [](const String &feed, const String &value) {receivedValue = value;});
}