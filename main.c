#include <regx52.h>
#include "lcd1602.h"
#include <string.h>
#include "matrix_key.h"
#include "calculator.h"
#include <stdio.h>
#include "ired.h"
// 按键:
// --------------------------------------------------

//             - - - - - - - - - - - - -
//             |  7  |  8  |  9  |  +  |
//             - - - - - - - - - - - - -
//             |  4  |  5  |  6  |  -  |
//             - - - - - - - - - - - - -
// 			   |  1  |  2  |  3  |  *  |
//             - - - - - - - - - - - - -
//             |  0  |  .  |  /  |  =  |
//             - - - - - - - - - - - - -

//             - - - - - - - - - - - - -
//             |  (  |  )  |  B  |shift| //默认, B：是退格
//             - - - - - - - - - - - - -
//             |  <  |  >  |  C  |shift| //<>光标移动，C：清屏
//             - - - - - - - - - - - - -

// 遥控16进制
// | 45 | 46 | 47 |    | C | S | O |
// | 44 | 40 | 43 |    | ( | ) | B |
// | 07 | 15 | 09 |    | + | - | x |
// | 16 | 19 | 0d |    | 0 | . | / |
// | 0c | 18 | 5e |    | 1 | 2 | 3 |
// | 08 | 1c | 5a |    | 4 | 5 | 6 |
// | 42 | 52 | 4a |    | 7 | 8 | 9 |

u8 code shiftKeys[] = {'<', '>', 'C', 'S'};
u8 code keys[] = {'7', '8', '9', '+', '4', '5', '6', '-', '1', '2', '3', 'x', '0', '.', '/', '=', '(', ')', 'B', 'S'};
u8 code iredCodeArr[] = {'7', '8', '9', '4', '5', '6', '1', '2', '3', '0', '.', '/', '+', '-', 'x', '(', ')', 'B', 'C', 'S', 'O'};

u8 isShiftMode = 0;
u8 isOpen = 1;
u8 xdata express[256];
u8 expressIndex = 0;
u8 expressLen = 0;
u8 cursor = 1;
u8 displayString[14];
u8 windowFirst = 0;

u8 getCharByKey(u8 keycode)
{
    if (!isShiftMode || keycode < 16)
    {
        return keys[keycode];
    }
    else
    {
        return shiftKeys[keycode - 16];
    }
}

void substring(char *str, int start, int end, char *sub)
{
    int n = end - start;
    strncpy(sub, str + start, n);
    sub[n] = '\0';
}

void toCursor(u8 cursor)
{
    u8 i;
    lcd1602_cursor_return();
    for (i = 0; i < cursor; i++)
    {
        lcd1602_cursor_shift(1);
    }
}

void showExpress(char *express)
{
    u8 expressLen = strlen(express);
    if (expressLen >= 14)
    {
        windowFirst = expressLen - 14;
        substring(express, windowFirst, expressLen, displayString);
        lcd1602_string_set(1, 0, displayString);
        cursor = 14;
    }
    else
    {
        substring(express, 0, expressLen, displayString);
        lcd1602_clear();
        if (expressLen == 0)
        {
            cursor = 1;
            toCursor(cursor);
            return;
        }

        lcd1602_string_set(1, 0, displayString);
        cursor = strlen(displayString) - 1;
    }
}

void removeChar(char str[], int index)
{
    int len = strlen(str);
    int i;
    for (i = index; i < len - 1; i++)
    {
        str[i] = str[i + 1];
    }
    str[len - 1] = '\0';
}

void hander(char option);

void handleIred(int iredCode)
{
    u8 option = iredCodeArr[iredCode];
    hander(option);
}

void main()
{
    u8 key, option, ired;
    lcd1602_init();
    lcd1602_display_switch(true, true, false);
    // ired_init();
    //     lcd1602_string_set(0,0,"1234567890abcde");
    // lcd1602_string_set(4,1,"world!");

    while (1)
    {
        //        continue;
        key = key_matrix_ranks_scan();
        if (key != -1)
        {
            option = getCharByKey(key);
            hander(option);
        }

        if (last_gired_data != -1)
        {
            handleIred(ired);
            last_gired_data = -1;
        }
    }
}

void hander(char option)
{
    if (option == 'S')
    {
        isShiftMode = !isShiftMode;
        lcd1602_char_set(0, 0, isShiftMode ? 's' : ' ');
        // lcd1602_char_set(0, 0, ' ');
        toCursor(cursor);
        return;
    }
    if (option == 'C')
    {
        lcd1602_clear();
        lcd1602_char_set(0, 0, isShiftMode ? 's' : ' ');
        cursor = 1;
        express[0] = '\0';
        expressIndex = 0;
    }

    if (option == '<')
    {
        if (cursor == 1)
            return;
        lcd1602_cursor_shift(0);
        cursor--;
        return;
    }

    if (option == '>')
    {
        if (cursor == 15)
            return;
        lcd1602_cursor_shift(1);
        cursor++;
        return;
    }

    if (option == 'B') // backspace
    {

        expressLen = strlen(express);
        if (expressLen == 0)
        {
            return;
        }
        removeChar(express, expressIndex);
        expressIndex--;
        showExpress(express);

        return;
    }

    if (option == '=')
    {
        double a = calculator(express);
        char result[16];
        u8 resultLen;
        sprintf(result, "%.10g", a);
        resultLen = strlen(result);

        lcd1602_string_set(0, 1, "                ");
        lcd1602_string_set(16 - resultLen, 1, result);
        toCursor(cursor + 2);
        return;
    }

    if (option == 'O')
    {
        if (isOpen)
        {
            lcd1602_display_switch(0, 0, 0);
        }
        else
        {
            lcd1602_display_switch(1, 1, 0);
        }
        isOpen = !isOpen;
        return;
    }

    if ((option >= '0' && option <= '9') || option == '+' || option == '-' || option == 'x' || option == '/' || option == '(' || option == ')' || option == '.')
    {
        if (strlen(express) >= 255)
        {
            lcd1602_string_set(0, 1, "max 255 char!");
            return;
        }
        express[expressIndex++] = option;
        express[expressIndex] = '\0';

        showExpress(express);
    }
}

void ired() interrupt 0 // 外部中断0服务函数
{
    parseIred();
}
