#ifndef _TYPE_KEY_MATRIX_H_
#define _TYPE_KEY_MATRIX_H_
 
#include <stdint.h>

typedef struct {
    uint8_t key_matrix_cur[7][6];  // 7x6的键盘矩阵
    uint8_t key_matrix_drumpad;    // 鼓垫的状态
} KeyMatrix;

#endif
