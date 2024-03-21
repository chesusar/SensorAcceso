#define BUTTON_PIN_BITMASK 0x200008000 // 2^33 + 2^15 in hex

RTC_DATA_ATTR int bootCount = 0;

void setup(){
  Serial.begin(115200);
  delay(100);
  
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

  Serial.println("A dormir...");
  esp_deep_sleep_start();

  Serial.println("Esto no se lee");
}

void loop(){
  // Esto no se lee
}
