#ifndef __LCD12864_H
#define __LCD12864_H

//---包含头文件---//
#include<reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---如果使用画图模式定义这个---//

//#define LCD12864_PICTURE

//---定义使用的IO口---//

#define LCD12864_DATAPORT P0	  //数据IO口

sbit LCD12864_RS  =  P2^5;             //（数据命令）寄存器选择输入 
sbit LCD12864_RW  =  P2^6;             //液晶读/写控制
sbit LCD12864_EN  =  P2^7;             //液晶使能控制
sbit LCD12864_PSB =  P2^4;   //P3^2;              //串/并方式控制
sbit LCD12864_RST =  P2^3;   //P3^4;				      //复位端



//---声明全局函数---//
void LCD12864_Delay1ms(uint c);
void Delay10ms(uint c);
uchar LCD12864_Busy(void);
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init();
void LCD12864_ClearScreen(void);
void LCD12864_SetWindow(uchar x, uchar y);
void LCD12864_DrowPic(uchar *a);
void LCD12864_clear();
void LCD12864_ShowString(uchar x,uchar y,const uchar *p);
#endif
