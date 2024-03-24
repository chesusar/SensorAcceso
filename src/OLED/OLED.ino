////Librerias pantalla////
#include "Pantallas.h"

////Instancias pantalla////
int frame = 0;

////Estados////
int estado = 3;
int inicio = 0;
int alerta = 1;
int clave = 2;
int pin_error = 3;
int desbloqueo = 4;

String pin = "987";

void setup() {
  Serial.begin(115200);

  ////Configurar pantalla////
  Config_pantalla();
}

void loop() {
  if(estado == inicio){
    pantalla_inicio();
  }
  
  if(estado == alerta){
    display.clearDisplay();
    display.drawBitmap(32, 0, icono1[frame], FRAME_WIDTH, FRAME_HEIGHT, 1);
    display.display();
    frame = (frame + 1) % FRAME_COUNT_icono1;
    delay(FRAME_DELAY); 
    
    /*
    if(){ // poner pulsar cualquier tecla
      estado = clave;
      frame = 0;
    }

    else if(){ // poner UID correcto
      estado = desbloqueo;
      frame = 0;
    }
    */    
  }

  if(estado == clave){
    pantalla_clave();

    display.setCursor(32, 33);     // Start at top-left corner
    display.print(pin[0]);
    display.setCursor(52, 33);     // Start at top-left corner
    display.print(pin[1]);
    display.setCursor(72, 33);     // Start at top-left corner
    display.print(pin[2]);
    display.setCursor(92, 33);     // Start at top-left corner
    display.print(pin[3]);
    display.display();
  }

  if(estado == pin_error){
    pantalla_pin_error();
  }

  if(estado == desbloqueo){
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
