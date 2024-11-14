#include "protocol_v1_decoder.h"
  
void protocol_v1_decoder_init(protocol_v1_decoder_t *decoder)
{
	memset(decoder, 0, sizeof(protocol_v1_decoder_t));
}

void protocol_v1_decoder_reset(protocol_v1_decoder_t *decoder)
{	
	decoder->status = E_LINK_DECODE_STA_RESET;
}

bool protocol_v1_decoder_run(protocol_v1_decoder_t *decoder, uint8_t ch)
{
    uint8_t ck_a = 0, ck_b = 0; // 初始化两个校验和变量

    switch (decoder->status) // 根据解码器当前状态进行操作
    {
        case E_LINK_DECODE_STA_RESET:  /* no break */ /* 解码器复位状态（没有break语句，继续执行下一状态） */
        case E_LINK_DECODE_STA_HEADER_A: // 解码器处于接收报文头部A的状态			
            if(ch == 0XAA) // 判断当前字符是否为0xAA（头部A的标志）
            {
                decoder->status = E_LINK_DECODE_STA_HEADER_B; // 如果是，则切换状态到接收头部B
            }
            else
            {
                decoder->status = E_LINK_DECODE_STA_RESET; // 如果不是，重置状态为复位状态
                protocol_v1_decoder_reset(decoder); // 调用复位函数，清空解码器
            }			
            break;
        case E_LINK_DECODE_STA_HEADER_B: // 解码器处于接收报文头部B的状态
            if(ch == 0XBB) // 判断当前字符是否为0xBB（头部B的标志）
            {
                decoder->status = E_LINK_DECODE_STA_CMD_SET; // 如果是，则切换状态到接收命令集
            }
            else if (ch == 0XAA) // 如果当前字符为0xAA
            {
                // 不做任何处理，保持当前状态
            }
            else
            {
                decoder->status = E_LINK_DECODE_STA_RESET; // 如果不是，重置状态为复位状态
                protocol_v1_decoder_reset(decoder); // 调用复位函数，清空解码器
            }				
            break;
        case E_LINK_DECODE_STA_CMD_SET: // 解码器处于接收命令集的状态
            decoder->cmd_set = ch; // 将当前字符赋值给命令集字段
            decoder->status = E_LINK_DECODE_STA_CMD_ID; // 切换状态到接收命令ID
            break;				
        
        case E_LINK_DECODE_STA_CMD_ID: // 解码器处于接收命令ID的状态
            decoder->cmd_id = ch; // 将当前字符赋值给命令ID字段
            decoder->status = E_LINK_DECODE_STA_CMD_LENGTH_H; // 切换状态到接收数据长度的高字节
            break;					
        
        case E_LINK_DECODE_STA_CMD_LENGTH_H: // 解码器处于接收数据长度高字节的状态
            decoder->index = 0; // 将数据索引初始化为0
            decoder->length_h = ch; // 将当前字符赋值给数据长度高字节字段
            decoder->status = E_LINK_DECODE_STA_CMD_LENGTH_L; // 切换状态到接收数据长度的低字节						
            break;			
        
        case E_LINK_DECODE_STA_CMD_LENGTH_L: // 解码器处于接收数据长度低字节的状态
            decoder->index = 0; // 再次将数据索引初始化为0
            decoder->length_l = ch; // 将当前字符赋值给数据长度低字节字段
            decoder->length = (decoder->length_h << 8) | decoder->length_l; // 计算总数据长度并赋值给length字段
            decoder->status = E_LINK_DECODE_STA_CMD_DATA; // 切换状态到接收数据部分					
            break;			
                
        case E_LINK_DECODE_STA_CMD_DATA: // 解码器处于接收数据部分的状态
            if (decoder->index < V1_DECODER_MAX_DATA_L) // 如果当前数据索引小于最大允许数据长度
            {
                decoder->data[decoder->index ++] = ch; // 将当前字符存储到数据数组中并增加索引
            }
            if ((decoder->index == decoder->length) // 如果索引等于数据长度
                || (decoder->index >= V1_DECODER_MAX_DATA_L)) // 或者索引超过最大数据长度
            {
                decoder->status = E_LINK_DECODE_STA_CMD_CHECK_SUM_A; // 切换状态到接收校验和A
            }								
            break;	
                
        case E_LINK_DECODE_STA_CMD_CHECK_SUM_A: // 解码器处于接收校验和A的状态
            decoder->ck_sum_a = ch; // 将当前字符赋值给校验和A字段
            decoder->status = E_LINK_DECODE_STA_CMD_CHECK_SUM_B; // 切换状态到接收校验和B
            break;	
        
        case E_LINK_DECODE_STA_CMD_CHECK_SUM_B: // 解码器处于接收校验和B的状态
            decoder->ck_sum_b = ch; // 将当前字符赋值给校验和B字段
            decoder->status = E_LINK_DECODE_STA_RESET; // 切换状态为复位状态（解码完成）
        
            // 开始进行校验和验证
            ck_a = ck_b = 0; // 初始化本地校验和变量
            ck_a += decoder->cmd_set; // 计算校验和A
            ck_b += ck_a; // 计算校验和B
            ck_a += decoder->cmd_id; // 累加命令ID到校验和A
            ck_b += ck_a; // 累加到校验和B		
            ck_a += decoder->length_h; // 累加长度高字节到校验和A
            ck_b += ck_a; // 累加到校验和B	
            ck_a += decoder->length_l; // 累加长度低字节到校验和A
            ck_b += ck_a; // 累加到校验和B				
            for(uint16_t i = 0; (i < decoder->length) && (i < V1_DECODER_MAX_DATA_L); i++) // 循环处理所有数据字节
            {
                ck_a += decoder->data[i]; // 累加数据字节到校验和A
                ck_b += ck_a; // 累加到校验和B
            }				
            if((ck_a == decoder->ck_sum_a) && (ck_b == decoder->ck_sum_b)) // 如果计算出的校验和与接收到的校验和匹配
            {				
                return true; // 返回true，表示解码成功
            }
            else
            {
                protocol_v1_decoder_reset(decoder); // 如果校验和不匹配，重置解码器
            }				
            break;		
        default :
            break;
    }
	
    return false; // 如果未能成功解码，返回false
}

 
 

 
