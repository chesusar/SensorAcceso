#include <Arduino.h>
#include <communication/communication.h>

#define BUTTON_PIN_BITMASK 0x200008000 // 2^33 + 2^16 en hex

const uint8_t slaveAddress[] = {0x24, 0x0A, 0xC4, 0xC5, 0xDD, 0x48};

// RTC_DATA_ATTR int bootCount = 0;

void setup()
{
  Serial.begin(115200);
  delay(100);

  // Serial.println("----Se despertó----");
  // Serial.println(bootCount);

  // if (true)
  // {
  Serial.println("BootCount > 0");
  communication_init(slaveAddress);
  delay(100);
  Serial.println("Comunicacion establecida");

  // }

  // ++bootCount;

  // esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low
  // esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ANY_HIGH);
  // delay(5000);
  // esp_deep_sleep_start();
}

void loop()
{
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
  delay(1000);
}
