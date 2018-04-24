#ifndef __DS1302_H_
#define __DS1302_H_

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


//---���峬����ģ��ʹ�õ�IO��---//
sbit Trig = P1^0;
sbit Echo = P1^1;

//---����ȫ�ֺ���---//
void Conut(void);
void delayms(unsigned int ms);
void InitWave();
void showdist(void);

//---����ȫ�ֱ���--//

#endif