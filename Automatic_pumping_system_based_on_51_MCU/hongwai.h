#ifndef __HONGWAI_H_
#define __HONGWAI_H_

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


//---定义使用的IO口---//
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit IRIN=P3^2;

//---定义全局函数---//
void IrInit();
void ReadIr();
void HongWai();

//---加入全局变量--//

#endif