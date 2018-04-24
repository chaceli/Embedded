#ifndef __IRLCD_H_
#define __IRLCD_H_

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

//---定义红外遥控器、蜂鸣器使用的IO口---//
sbit IRIN = P3^2;         //红外接收器数据线
sbit RELAY= P1^4;         //继电器驱动线
sbit BEEP = P1^5;         //蜂鸣器驱动线
//sbit LED  = P1^0;					//测试LED

//---定义全局函数---//
void Timer1Init();
void IrInit();
void irlcd();
void IR_IN(void);
void delay(uchar x);  //x*0.14MS
void delay1(int ms);
void beep(void);

//---加入全局变量--//
extern uchar flag;

#endif