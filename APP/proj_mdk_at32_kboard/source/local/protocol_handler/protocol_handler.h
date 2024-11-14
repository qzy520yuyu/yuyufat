#ifndef PROTOCOL_HANDLER_H
#define PROTOCOL_HANDLER_H

#include <stdint.h>

// 定义TLV结构体
typedef struct {
    uint8_t type;
    uint8_t length;
    uint8_t *value;
} TLV;

// 函数声明
int parse_tlv(const uint8_t *data, int data_length, TLV *parsed_tlv);
void print_tlv(const TLV *tlv);
void tlvc_regist();
#endif // ANAL_DEVICE_H
