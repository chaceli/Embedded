/********************************************************************************
*  描述:                                                                       *
*         lcd1602显示 遥控键值读取器                                           *
*         lcd1602显示 遥控器接p3.2                                             *
*         喇叭接p1.5  继电器接p1.4                                             *
*   连接方法：使用红外功能时 J1跳线短接                                                                           *
*******************************************************************************/

#include <reg51.h>
#include "irlcd.h"
#include <intrins.h>
#include "lcd12864.h"
#include "ds1302.h"


#define  NOP()   _nop_()   /* 定义空指令 */

#define uchar unsigned char
#define uint  unsigned int
#define delayNOP(); {_nop_();_nop_();_nop_();_nop_();};

uchar IRCOM[7];
int m=1;
unsigned char Y0;
uchar flag=1;

/**********************************************************/
void delay(uchar x)    //x*0.14MS
{
 unsigned char i;
  while(x--)
 {
  for (i = 0; i<13; i++) {}
 }
}

/**********************************************************/
void delay1(int ms)
{
 unsigned char y;
  while(ms--)
 {
  for(y = 0; y<250; y++)
  {
   _nop_();
   _nop_();
   _nop_();
   _nop_();
  }
 }
}

/*******************************************************************************
* 函数名         : IrInit()
* 函数功能		   : 初始化红外线接收
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void IrInit()
{
	TMOD|=0x10;		   //设T0为方式1，GATE=1；
	IT0=1;//下降沿触发
	EX0=1;//打开中断0允许
	EA =1;	//打开总中断

	IRIN =1;//初始化端口
  RELAY=1;
  BEEP =1;
	
	IE = 0x81;                 //允许总中断中断,使能 INT0 外部中断
	TCON = 0x01;               //触发方式为脉冲负边沿触发
}

/*******************************************************************/
void irlcd()
{
    delay1(10);                 //延时
		Delay10ms(10);
		LCD12864_ShowString(2,0,"状态：");
	
/*------------------------------------------------
           		抽水显示
------------------------------------------------*/
		switch(flag)
		{
			case 0:LCD12864_ShowString(2,3,"Pumping");break;
			case 1:LCD12864_ShowString(2,3,"Enough~");break;
			default:break;
		}	
		
		Delay10ms(10);
} 
/**********************************************************/
void IR_IN(void) interrupt 0 	 //外部中断服务程序
{
  unsigned char j,k,N=0;
     EX0 = 0;   
	 delay(15);
	 if (IRIN==1) 
     { EX0 =1;
	   return;
	  } 
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {delay(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {delay(1);}
    while (!IRIN)          //等 IR 变为高电平
     {delay(1);}
     while (IRIN)           //计算IR高电平时长
      {
    delay(1);
    N++;           
    if (N>=30)
	 { EX0=1;
	 return;}                  //0.14ms计数过长自动离开。
      }                        //高电平计数完毕                
     IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
  }//end for k
 }//end for j
   
   if (IRCOM[2]!=~IRCOM[3])
   { EX0=1;
     return; }

   IRCOM[5]=IRCOM[2] & 0x0F;     //取键码的低四位
   IRCOM[6]=IRCOM[2] >> 4;       //右移4次，高四位变为低四位

   if(IRCOM[5]>9)
    { IRCOM[5]=IRCOM[5]+0x37;}
   else
	  IRCOM[5]=IRCOM[5]+0x30;

   if(IRCOM[6]>9)
    { IRCOM[6]=IRCOM[6]+0x37;}
   else
	  IRCOM[6]=IRCOM[6]+0x30;

//     LCD12864_SetWindow(2,0);             
//     LCD12864_WriteData(IRCOM[6]);        //第一位数显示             
//     LCD12864_WriteData(IRCOM[5]);        //第二位数显示

/*------------------------------------------------
        将键值转化为ASCII值   
------------------------------------------------*/
		
	 Y0=0;
	 switch(IRCOM[2])
	 {
		 case 0x16: Y0=0x00; break;
		 case 0x0C: Y0=0x01; break;
	   case 0x18: Y0=0x02; break;
	   case 0x5E: Y0=0x03; break;
	   case 0x08: Y0=0x04; break;
	   case 0x1C: Y0=0x05; break;
	   case 0x5A: Y0=0x06; break;
	   case 0x42: Y0=0x07; break;
	   case 0x52: Y0=0x08; break;
	   case 0x4A: Y0=0x09; break;
//	   case 0x40:  left=1; break;
//	   case 0x43: right=1; break;
//		 case 0x15: voll =1; break;
//	   case 0x09: volr =1; break;
	 }	 
	 
/*------------------------------------------------
     			控制继电器开关
------------------------------------------------*/
		if((IRCOM[2]==0x45)&&(m==1))
		{
			m=0;
			IRCOM[2]=0x00;
			RELAY = 0;
			flag=0;
			Delay10ms(1);
		if((IRCOM[2]==0x45)&&(m==0))
		{
			m=1;
			IRCOM[2]=0x00;
			RELAY=1;
			flag=1;
		} 

		beep();
		EX0 = 1; 
		} 
}

/**********************************************************/
void beep(void)
{
  unsigned char i;
  for (i=0;i<100;i++)
   {
   delay(4);
   BEEP=!BEEP;                 //BEEP取反
   } 
  BEEP=1;                      //关闭蜂鸣器
}

/*******************************************************************************
* 函 数 名         : Timer0Init
* 函数功能		   	 : 定时器0初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer1Init()
{
	TMOD|=0X10;//选择为定时器0模式，工作方式1，仅用TR0打开启动。
	TH1=0XFC;	//给定时器赋初值，定时1ms
	TL1=0X18;   
  ET0=1;                                   
	TR1=1;//打开定时器	
  EA=1;	
}

/*******************************************************************************
* 函 数 名         : void Timer0() interrupt 3
* 函数功能			   : 定时器0中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer1() interrupt 3
{
	TH1=0XFC;	//给定时器赋初值，定时1ms
	TL1=0X18;
	if((IRCOM[2]==0x45)&&(m==1))
	{
		m=0;
		RELAY = 0;
	}
	if((IRCOM[2]==0x45)&&(m==0))
	{
		m=1;
		RELAY=1;
	}
}