#ifndef MATRIX_KEY_H
#define MATRIX_KEY_H

#include <regx52.h>

typedef unsigned int u16; // 对系统默认数据类型进行重定义
typedef unsigned char u8;

#define KEY_MATRIX_PORT P1 // 使用宏定义矩阵按键控制口

u8 key_matrix_ranks_scan(void);

u8 key_matrix_flip_scan(void);

/*******************************************************************************
 * 函 数 名       : main
 * 函数功能		 : 主函数
 * 输    入       : 无
 * 输    出    	 : 无
 *******************************************************************************/
/*void main()
{
    u8 key=0;

    while(1)
    {
        key=key_matrix_ranks_scan();
        if(key!=0)
            SMG_A_DP_PORT=gsmg_code[key-1];//得到的按键值减1换算成数组下标对应0-F段码
    }
}*/