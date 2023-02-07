#include "matrix_key.h"
/*******************************************************************************
* 函 数 名       : delay_10us
* 函数功能		 : 延时函数，ten_us=1时，大约延时10us
* 输    入       : ten_us
* 输    出    	 : 无
*******************************************************************************/
void delay_10us(u16 ten_us)
{
	while(ten_us--);	
}

/*******************************************************************************
* 函 数 名       : key_matrix_ranks_scan
* 函数功能		 : 使用行列式扫描方法，检测矩阵按键是否按下，按下则返回对应键值
* 输    入       : 无
* 输    出    	 : key_value：0-15，对应S1-S16键，
				   -1：按键未按下
*******************************************************************************/
u8 key_matrix_ranks_scan(void)
{
	u8 key_value=-1;

	KEY_MATRIX_PORT=0xf7;//给第一列赋值0，其余全为1
	if(KEY_MATRIX_PORT!=0xf7)//判断第一列按键是否按下
	{
		delay_10us(1000);//消抖
		switch(KEY_MATRIX_PORT)//保存第一列按键按下后的键值	
		{
			case 0x77: key_value=0;break;
			case 0xb7: key_value=4;break;
			case 0xd7: key_value=8;break;
			case 0xe7: key_value=12;break;
		}
	}
	while(KEY_MATRIX_PORT!=0xf7);//等待按键松开	
	
	KEY_MATRIX_PORT=0xfb;//给第二列赋值0，其余全为1
	if(KEY_MATRIX_PORT!=0xfb)//判断第二列按键是否按下
	{
		delay_10us(1000);//消抖
		switch(KEY_MATRIX_PORT)//保存第二列按键按下后的键值	
		{
			case 0x7b: key_value=1;break;
			case 0xbb: key_value=5;break;
			case 0xdb: key_value=9;break;
			case 0xeb: key_value=13;break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfb);//等待按键松开	
	
	KEY_MATRIX_PORT=0xfd;//给第三列赋值0，其余全为1
	if(KEY_MATRIX_PORT!=0xfd)//判断第三列按键是否按下
	{
		delay_10us(1000);//消抖
		switch(KEY_MATRIX_PORT)//保存第三列按键按下后的键值	
		{
			case 0x7d: key_value=2;break;
			case 0xbd: key_value=6;break;
			case 0xdd: key_value=10;break;
			case 0xed: key_value=14;break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfd);//等待按键松开	
	
	KEY_MATRIX_PORT=0xfe;//给第四列赋值0，其余全为1
	if(KEY_MATRIX_PORT!=0xfe)//判断第四列按键是否按下
	{
		delay_10us(1000);//消抖
		switch(KEY_MATRIX_PORT)//保存第四列按键按下后的键值	
		{
			case 0x7e: key_value=3;break;
			case 0xbe: key_value=7;break;
			case 0xde: key_value=11;break;
			case 0xee: key_value=15;break;
		}
	}
	while(KEY_MATRIX_PORT!=0xfe);//等待按键松开
	
	if (!P3_1)
	{
		delay_10us(1000);//消抖
		while (!P3_1);
		delay_10us(1000);//消抖
		key_value=16;
	}
	
	if (!P3_0)
	{
		delay_10us(1000);//消抖
		while (!P3_0);
		delay_10us(1000);//消抖
		key_value=17;
	}

	if (!P3_2)
	{
		delay_10us(1000);//消抖
		while (!P3_2);
		delay_10us(1000);//消抖
		key_value=18;
	}

	if (!P3_3)
	{
		delay_10us(1000);//消抖
		while (!P3_3);
		delay_10us(1000);//消抖
		key_value=19;
	}
	return key_value;		
}

/*******************************************************************************
* 函 数 名       : key_matrix_flip_scan
* 函数功能		 : 使用线翻转扫描方法，检测矩阵按键是否按下，按下则返回对应键值
* 输    入       : 无
* 输    出    	 : key_value：1-16，对应S1-S16键，
				   0：按键未按下
*******************************************************************************/
u8 key_matrix_flip_scan(void)
{
	static u8 key_value=0;

	KEY_MATRIX_PORT=0x0f;//给所有行赋值0，列全为1
	if(KEY_MATRIX_PORT!=0x0f)//判断按键是否按下
	{
		delay_10us(1000);//消抖
		if(KEY_MATRIX_PORT!=0x0f)
		{
			//测试列
			KEY_MATRIX_PORT=0x0f;
			switch(KEY_MATRIX_PORT)//保存行为0，按键按下后的列值	
			{
				case 0x07: key_value=0;break;
				case 0x0b: key_value=1;break;
				case 0x0d: key_value=2;break;
				case 0x0e: key_value=3;break;
			}
			//测试行
			KEY_MATRIX_PORT=0xf0;
			switch(KEY_MATRIX_PORT)//保存列为0，按键按下后的键值	
			{
				case 0x70: key_value=key_value;break;
				case 0xb0: key_value=key_value+4;break;
				case 0xd0: key_value=key_value+8;break;
				case 0xe0: key_value=key_value+12;break;
			}
			while(KEY_MATRIX_PORT!=0xf0);//等待按键松开	
		}
	}
	else
		key_value=0;		
	
	return key_value;		
}