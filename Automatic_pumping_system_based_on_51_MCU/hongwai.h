#ifndef __HONGWAI_H_
#define __HONGWAI_H_

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


//---����ʹ�õ�IO��---//
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

sbit IRIN=P3^2;

//---����ȫ�ֺ���---//
void IrInit();
void ReadIr();
void HongWai();

//---����ȫ�ֱ���--//

#endif