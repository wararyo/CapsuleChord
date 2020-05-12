#ifndef _MIDINOTE_H_
#define _MIDINOTE_H_

#include <stdint.h>

typedef struct {
    uint8_t status;
    uint8_t noteNo;
    uint8_t velocity;
} MidiNote;

#endif