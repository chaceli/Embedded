#ifndef __COM_H_
#define __COM_H_

//---包含头文件---//
#include<reg51.h>

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---定义全局函数---//
void tempshow();
void temp(int);
void UsartConfiguration();

#endif