#ifndef __DS1302_H_
#define __DS1302_H_

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


//---定义超声波模块使用的IO口---//
sbit Trig = P1^0;
sbit Echo = P1^1;

//---定义全局函数---//
void Conut(void);
void delayms(unsigned int ms);
void InitWave();
void showdist(void);

//---加入全局变量--//

#endif