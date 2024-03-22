#include "input.h"
#include <Arduino.h>

static uint8_t columns[4];
static uint8_t rows[4];

static char keys[] = "123A456B789C*0#D";

void input_init(uint8_t *columnPins, uint8_t *rowPins)
{
    memcpy(columns, columnPins, 4);
    memcpy(rows, rowPins, 4);

    for (uint8_t c = 0; c < 4; c++)
        pinMode(columns[c], OUTPUT);
    for (uint8_t c = 0; c < 4; c++)
        pinMode(rows[c], INPUT);
}

char input_getChar()
{
    static char oldKey = KEY_NULL;
    char key = KEY_NULL;

    for (uint8_t c = 0; c < 4; c++)
    {
        for (uint8_t c2 = 0; c2 < 4; c2++)
            digitalWrite(columns[c2], LOW);
        digitalWrite(columns[c], HIGH);
        for (uint8_t r = 0; r < 4; r++)
        {
            if (digitalRead(rows[r]) == HIGH)
            {
                key = keys[c + r * 4];
            }
        }
    }

    if (key == oldKey)
    {
        oldKey = key;
        return KEY_NULL;
    }
    else
    {
        oldKey = key;
        return key;
    }
}
