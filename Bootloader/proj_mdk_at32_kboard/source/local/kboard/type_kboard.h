#ifndef _TYPE_Kboard_H_
#define _TYPE_Kboard_H_
 
#include <stdint.h>

typedef struct
{
    uint8_t 	note[12];        // 存储音符的数组，长度为7个字节，每个字节代表一个音符
    uint8_t 	key_0[12];       // 存储键盘第0组按键状态的数组，长度为7个字节
    uint8_t 	key_1[12];       // 存储键盘第1组按键状态的数组，长度为7个字节
    uint8_t 	arpeggio[6];    // 存储琶音设置的数组，长度为6个字节
    uint8_t 	drumpad;        // 存储鼓垫（Drum Pad）的状态，1个字节
    uint8_t 	bias;           // 存储偏置值（可能用于音调或其他参数调整），1个字节
} kboard_t;                    // 定义结构体类型 kboard_t，表示键盘（keyboard）的相关参数和状态

#endif
