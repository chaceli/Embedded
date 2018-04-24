/*******************************************************************************
*	项目名称		   : 基于51单片机的液位检测装置
* 使用的IO	     : 
* 实验效果       :按K3进入时钟设置，按K1选择设置的时分秒日月，按K2选择选择设置加1
*	注意					 ：
*******************************************************************************/

#include <reg51.h>
#include "lcd12864.h"
#include "ds1302.h"
#include "key.h"
#include "shengbo.h"
#include "com.h"
#include "ds18b20.h"
#include "irlcd.h"


/*******************************************************************************
* 函数名         : main
* 函数功能		   : 主函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void main()
{
	LCD12864_Init();		//LCD12864初始化  
//	Ds1302Init();
	Timer1Init();
  init_t0();
	init_measuring();   //超声波相应端口初始化
	IrInit();						//IR红外模块初始化
	UsartConfiguration();  //初始化串口
	Ds18b20Init();				 //初始化温度传感器
	Int1Init();
	while(1)
	{
		LcdDisplay();
		Water_Level();
		irlcd();
		key();
		tempshow();
	}
}

void T_0()  interrupt 1
{
    TF0 = 0;
    TL0 = 0x66;
    TH0 = 0xfc;

    count++;
    if(count==18)
    {
      TR0 =0;
      TL0 = 0x66;
      TH0 = 0xfc;
      count = 0;
    }
}