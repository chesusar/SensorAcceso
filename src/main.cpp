#include <Arduino.h>
#include <WiFi.h>
#include "communication.h"
#include "input.h"

// Slave COM5
// Host COM14

uint8_t hostAddress[] = {0xF0, 0x08, 0xD1, 0xD8, 0x1C, 0x18};
uint8_t slaveAddress[] = {0xF0, 0x08, 0xD1, 0xD8, 0x1C, 0x18};

void setup()
{
  Serial.begin(9600);

  WiFi.mode(WIFI_MODE_STA);
  Serial.println("Iniciando wifi");
  Serial.println(WiFi.macAddress());

  // communication_init(NULL);
  // communication_init(slaveAddress);

  uint8_t rowPins[] = {25, 33, 32, 35};
  uint8_t columnPins[] = {12, 14, 27, 26};
  input_init(columnPins, rowPins);
}

void loop()
{
  char input = input_getChar();
  if (input != KEY_NULL)
  {
    if (input != '#')
      Serial.print(input);
    else
      Serial.println("");
  }
  delay(50);

  // if (!communication_isHost())
  //   return;
  // char buff[20];
  // communication_send(slaveAddress, 1, itoa(random(0, 1000), buff, 10));
  // delay(1000);
}
