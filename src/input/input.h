#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

#define KEY_NULL (char)'\0'

/**
 * @brief inits input on selected pins
 * @param columnPins column output pins
 * @param rowPins row input pins
 */
void input_init(uint8_t *columnPins, uint8_t *rowPins);

/**
 * @brief gets last unique char or KEY_NULL
 * @return last char or KEY_NULL
 */
char input_getChar();

#endif