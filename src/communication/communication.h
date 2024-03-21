#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <cstdlib>
#include <esp_now.h>

typedef struct message
{
    uint32_t time;
    uint8_t type;
    char message[100];
} message;

/**
 * @brief Inits communication server
 * @param slaveAddress slave address to use in host mode, NULL to use as slave
 */
void communication_init(uint8_t *slaveAddress);

/**
 * @brief Is communication server active
 * @returns true if communication server is active
 */
bool communication_isActive();

/**
 * @brief Is communication server host
 * @returns true if communication server is host
 */
bool communication_isHost();

/**
 * @brief Sends message to address
 * @param address address to send message to
 * @param type type of message
 * @param msg message
 */
void communication_send(const uint8_t *address, uint8_t type, const char *msg);

#endif