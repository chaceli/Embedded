
#include <reg51.h>
#include <intrins.h>	
#include "lcd12864.h"
#include "wave.h"


unsigned char PuZh[]=" Pechin Science ";
unsigned char code ASCII[15] =    {'0','1','2','3','4','5','6','7','8','9','.','-','M'};

static unsigned char DisNum = 0; //显示用指针				  
unsigned int  checktime=0;
unsigned long S=0;
bit      flag =0;
unsigned char disbuff[4]={ 0,0,0,0,};
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Conut(void)
	{
	 checktime=TH0*256+TL0;
	 TH0=0;
	 TL0=0;
	
	 S=(checktime*1.7)/100;     //算出来是CM
	 if((S>=700)||flag==1) //超出测量范围显示“-”
	 {	 
	  flag=0;
		LCD12864_ShowString(1,0,"液面:-.--M");
//		LCD12864_WriteData("-.--M");
//	  DisplayOneChar(0, 1, ASCII[11]);
//	  DisplayOneChar(1, 1, ASCII[10]);	//显示点
//	  DisplayOneChar(2, 1, ASCII[11]);
//	  DisplayOneChar(3, 1, ASCII[11]);
//	  DisplayOneChar(4, 1, ASCII[12]);	//显示M
	 }
	 else
	 {
	  disbuff[0]=S%1000/100;
	  disbuff[1]=S%1000%100/10;
	  disbuff[2]=S%1000%10 %10;
		 LCD12864_ShowString(1,0,"液面:");
	  LCD12864_WriteData(ASCII[disbuff[0]]);
	  LCD12864_WriteData(ASCII[10]);	//显示点
	  LCD12864_WriteData(ASCII[disbuff[1]]);
	  LCD12864_WriteData(ASCII[disbuff[2]]);
	  LCD12864_WriteData(ASCII[12]);	//显示M
	 }
	}

void zd0() interrupt 1 		 //T0中断用来计数器溢出,超过测距范围
  {
    flag=1;							 //中断溢出标志
  }

 void  StartModule() 		         //启动模块
  {
	  Trig=1;			                     //启动一次模块
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  _nop_(); 
	  _nop_(); 
	  _nop_(); 
	  _nop_();
	  Trig=0;
  }



void delayms(unsigned int ms)
{
	unsigned char i=100,j;
	for(;ms;ms--)
	{
		while(--i)
		{
			j=10;
			while(--j);
		}
	}
}

void InitWave()
{
	 TMOD=0x01;		 //设T0为方式1，GATE=1；
	 TH0=0;
	 TL0=0;          
	 ET0=1;        //允许T0中断
	 EA=1;			   //开启总中断	
}

void showdist(void)
{
//	while(1)
//	{
		 StartModule();
	     while(!Echo);		//当RX为零时等待
	     TR0=1;			    //开启计数
	     while(Echo);			//当RX为1计数并等待
	     TR0=0;				//关闭计数
         Conut();			//计算
		 delayms(80);
//	}	
}