#include "Comunicacion.h"

#define BUTTON_PIN_BITMASK 0x200008000 // 2^33 + 2^15 en hex 

uint8_t slaveAddress[] = {0x24, 0x0A, 0xC4, 0xC5, 0xDD, 0x48};  // alarma COM 11

// Sensor_puerta => pin 33
// Sensor_pir => pin 15

RTC_DATA_ATTR int bootCount = 0;

void setup(){
  Serial.begin(115200);
  delay(100);

  if(bootCount > 0){
    communication_init(slaveAddress);
    delay(100);
    
    char buff[20];
    communication_send(slaveAddress, 1, itoa(1, buff, 10));
    delay(200);
    communication_send(slaveAddress, 1, itoa(1, buff, 10));
    delay(200);
    communication_send(slaveAddress, 1, itoa(1, buff, 10));
    delay(200);
    communication_send(slaveAddress, 1, itoa(1, buff, 10));
    delay(200);
    communication_send(slaveAddress, 1, itoa(1, buff, 10));
    delay(200);
  }
  
  ++bootCount;
  
  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);
  esp_deep_sleep_start();

  // Esto no se lee
  
}

void loop(){
  // Esto no se lee
}
