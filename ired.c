#include "ired.h"
// 遥控16进制
// | 45 | 46 | 47 |    | C | S | O |
// | 44 | 40 | 43 |    | ( | ) | B |
// | 07 | 15 | 09 |    | + | - | x |
// | 16 | 19 | 0d |    | 0 | . | / |
// | 0c | 18 | 5e |    | 1 | 2 | 3 |
// | 08 | 1c | 5a |    | 4 | 5 | 6 |
// | 42 | 52 | 4a |    | 7 | 8 | 9 |

typedef unsigned char u8;
typedef unsigned int u16;
u8 gired_data[4];
unsigned char last_gired_data = -1;

void delay_10us_1(u16 ten_us)
{
    while (ten_us--)
        ;
}

unsigned char convIred(unsigned char iredCode)
{
    switch (iredCode)
    {
    case 0x42:
        return 0;
    case 0x52:
        return 1;
    case 0x4a:
        return 2;
    case 0x08:
        return 3;
    case 0x1C:
        return 4;
    case 0x5A:
        return 5;

    case 0x0C:
        return 6;
    case 0x18:
        return 7;
    case 0x5E:
        return 8;
    case 0x16:
        return 9;
    case 0x19:
        return 10;
    case 0x0D:
        return 11;

    case 0x07:
        return 12;
    case 0x15:
        return 13;
    case 0x09:
        return 14;
    case 0x44:
        return 15;
    case 0x40:
        return 16;
    case 0x43:
        return 17;

    case 0x45:
        return 18;
    case 0x46:
        return 19;
    case 0x47:
        return 20;
    default:
        return -1;
    }
}

void ired_init(void)
{
    IT0 = 1;  // 下降沿触发
    EX0 = 1;  // 打开中断0允许
    EA = 1;   // 打开总中断
    IRED = 1; // 初始化端口
}

void parseIred()
{
    u8 ired_high_time = 0;
    u16 time_cnt = 0;
    u8 i = 0, j = 0;

    if (IRED == 0)
    {
        time_cnt = 1000;
        while ((!IRED) && (time_cnt)) // 等待引导信号9ms低电平结束，若超过10ms强制退出
        {
            delay_10us_1(1); // 延时约10us
            time_cnt--;
            if (time_cnt == 0)
                return;
        }
        if (IRED) // 引导信号9ms低电平已过，进入4.5ms高电平
        {
            time_cnt = 500;
            while (IRED && time_cnt) // 等待引导信号4.5ms高电平结束，若超过5ms强制退出
            {
                delay_10us_1(1);
                time_cnt--;
                if (time_cnt == 0)
                    return;
            }
            for (i = 0; i < 4; i++) // 循环4次，读取4个字节数据
            {
                for (j = 0; j < 8; j++) // 循环8次读取每位数据即一个字节
                {
                    time_cnt = 600;
                    while ((IRED == 0) && time_cnt) // 等待数据1或0前面的0.56ms结束，若超过6ms强制退出
                    {
                        delay_10us_1(1);
                        time_cnt--;
                        if (time_cnt == 0)
                            return;
                    }
                    time_cnt = 20;
                    while (IRED) // 等待数据1或0后面的高电平结束，若超过2ms强制退出
                    {
                        delay_10us_1(10); // 约0.1ms
                        ired_high_time++;
                        if (ired_high_time > 20)
                            return;
                    }
                    gired_data[i] >>= 1;     // 先读取的为低位，然后是高位
                    if (ired_high_time >= 8) // 如果高电平时间大于0.8ms，数据则为1，否则为0
                        gired_data[i] |= 0x80;
                    ired_high_time = 0; // 重新清零，等待下一次计算时间
                }
            }
        }
        if (gired_data[2] != ~gired_data[3]) // 校验控制码与反码，错误则返回
        {
            for (i = 0; i < 4; i++)
                gired_data[i] = 0;
            return;
        }
        last_gired_data = gired_data[2];
    }
}