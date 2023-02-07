#ifndef _ired_H
#define _ired_H

#include <regx52.h>
// 管脚定义
sbit IRED = P3 ^ 2;

// 声明变量
extern unsigned char gired_data[4];

extern unsigned char last_gired_data;

// 函数声明
void ired_init(void);

unsigned char convIred(unsigned char iredCode);
void parseIred();
#endif
