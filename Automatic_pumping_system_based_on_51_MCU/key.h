#ifndef __KEY_H_
#define __KEY_H_

//---包含头文件---//
#include<reg51.h>
#include<intrins.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---定义key使用的IO口---//
sbit K1=P2^2;
sbit K2=P2^1;
sbit K3=P3^3;
sbit K4=P2^0;

//---定义全局函数---//
void Int1Init();
void Int1();
void key();
void delay1ms(void);

//---加入全局变量--//

#endif