#include "communication.h"
#include <Arduino.h>
#include <esp_now.h>

static bool serverCommunication = false;
static bool isHost = false;

static void onSent(const uint8_t *mac_addr, esp_now_send_status_t status);
static void onReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len);

void communication_init(uint8_t *slaveAddress)
{
    if (slaveAddress == NULL)
        isHost = false;
    else
        isHost = true;
    // Init ESP NOW
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error iniciando ESP NOW");
    }
    serverCommunication = true;
    if (!communication_isActive())
        return;

    // Init Server/client
    if (communication_isHost())
    {
        esp_now_register_send_cb(onSent);

        esp_now_peer_info_t peerInfo = {};
        memcpy(peerInfo.peer_addr, slaveAddress, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;
        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.println("No se pudo añadir peer");
        }
    }
    else
    {
        esp_now_register_recv_cb(onReceived);
    }
}

bool communication_isActive()
{
    return serverCommunication;
}

bool communication_isHost()
{
    return isHost;
}

void communication_send(const uint8_t *address, uint8_t type, const char *msg)
{
    message m = {};
    strcpy(m.message, msg);
    m.time = millis();
    m.type = type;

    esp_err_t result = esp_now_send(address, (uint8_t *)&m, sizeof(m));
    if (result == ESP_OK)
    {
        Serial.println("Enviado");
    }
}

static void onSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.println("Communication sent");
}

static void onReceived(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    message m = {};
    memcpy(&m, data, sizeof(m));
    Serial.println("Received");
    Serial.print(m.time);
    Serial.print(" ");
    Serial.print(m.type);
    Serial.print(" ");
    Serial.println(String(m.message));
}
