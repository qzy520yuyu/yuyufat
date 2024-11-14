#ifndef _TYPE_Kboard_H_
#define _TYPE_Kboard_H_
 
#include <stdint.h>

typedef struct
{
    uint8_t 	note[12];
    uint8_t 	key_0[12];
    uint8_t 	key_1[12];
    uint8_t   arpstate[7];
    uint8_t 	bias;
} kboard_t;

#endif
