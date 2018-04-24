#ifndef __SHENGBO_H_
#define __SHENGBO_H_

//---包含头文件---//
#include<reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---定义超声波测距使用的IO口---//
sbit trig=P1^6;     //超声波测距模块Trig
sbit echo=P1^7;     //超声波测距模块Echo
extern uchar count; 
//---定义全局函数---//
void init_t0();
void trigger();
void delay(uchar n);
void lcd_xianshi();
void Water_Level();	
void display(uint x);
void measuring();
void init_measuring();


#endif