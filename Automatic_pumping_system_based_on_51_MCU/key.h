#ifndef __KEY_H_
#define __KEY_H_

//---����ͷ�ļ�---//
#include<reg51.h>
#include<intrins.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---����keyʹ�õ�IO��---//
sbit K1=P2^2;
sbit K2=P2^1;
sbit K3=P3^3;
sbit K4=P2^0;

//---����ȫ�ֺ���---//
void Int1Init();
void Int1();
void key();
void delay1ms(void);

//---����ȫ�ֱ���--//

#endif