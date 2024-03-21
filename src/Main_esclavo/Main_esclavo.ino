#define BUTTON_PIN_BITMASK 0x200008000 // 2^33 + 2^15 en hex 

// Sensor_puerta => pin 33
// Sensor_pir => pin 15

RTC_DATA_ATTR int bootCount = 0;

void setup(){
  Serial.begin(115200);
  delay(100);

  if(bootCount > 0){
    // Añadir codigo para enviar alerta
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
