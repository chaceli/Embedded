#include <reg51.h>
#include "lcd12864.h"
#include "ds1302.h"
#include "key.h"

void Int1Configuration();
void LcdDisplay();
uchar SetPlace;
bit SetState=0;
/*******************************************************************************
* 函数名         : key
* 函数功能		 	 : 按键设置时间
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void key()
{
    unsigned char i;
	if(SetState==0)
	{
		Ds1302ReadTime();
	}
	else if(SetState==1)
		{
			if(K1==0)		//检测按键K1是否按下
			{
				Delay10ms(1);	//消除抖动
				if(K1==0)
				{
					SetPlace++;
					if(SetPlace>=7)
						SetPlace=0;					
				}

				while((i<50)&&(K1==0))	 //检测按键是否松开
				{
					Delay10ms(1);
					i++;
				}
				i=0;
			}
			if(K2==0)		//检测按键K2是否按下
			{
				Delay10ms(1);	//消除抖动
				if(K2==0)
				{
					TIME[SetPlace]++;
					if((TIME[SetPlace]&0x0f)>9)					 //换成BCD码。
					{
						TIME[SetPlace]=TIME[SetPlace]+6;
					}
					if((TIME[SetPlace]>=0x60)&&(SetPlace<2))		//分秒只能到59
					{
						TIME[SetPlace]=0;
					}
					if((TIME[SetPlace]>=0x24)&&(SetPlace==2))		//小时只能到23
					{
						TIME[SetPlace]=0;
					}
					if((TIME[SetPlace]>=0x32)&&(SetPlace==3))		//日只能到31
					{
						TIME[SetPlace]=0;	
					}
					if((TIME[SetPlace]>=0x13)&&(SetPlace==4))		//月只能到12
					{
						TIME[SetPlace]=0;
					}	
					if((TIME[SetPlace]>=0x7)&&(SetPlace==5))		//周只能到7
					{
						TIME[SetPlace]=1;
					}		
//					if(SetPlace==5)		//月只能到12
//					{
//						TIME[SetPlace]=;
//					}	
				}
				while((i<50)&&(K2==0))	 //检测按键是否松开
				{
					Delay10ms(1);
					i++;
				}
				i=0;
		    Ds1302Init();
			}						
		}
}

/*******************************************************************************
* 函数名         : Int1Configuration()
* 函数功能			 : 配置外部中断1
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Int1Init()
{
	//设置INT1
	IT1=1;//跳变沿出发方式（下降沿）
	EX1=1;//打开INT1的中断允许。
	EA=1;//打开总中断	
}

/*******************************************************************************
* 函数名         : Int1() 
* 函数功能			 : 外部中断1中断函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Int1() interrupt 2		 
{
	if(K3==0)
	{
		Delay10ms(1);
		if(K3==0)
		{
		SetState=~SetState;
		SetPlace=0;
		Ds1302ReadTime();
		}			
	}
}