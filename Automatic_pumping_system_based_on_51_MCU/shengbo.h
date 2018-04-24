#ifndef __SHENGBO_H_
#define __SHENGBO_H_

//---����ͷ�ļ�---//
#include<reg51.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---���峬�������ʹ�õ�IO��---//
sbit trig=P1^6;     //���������ģ��Trig
sbit echo=P1^7;     //���������ģ��Echo
extern uchar count; 
//---����ȫ�ֺ���---//
void init_t0();
void trigger();
void delay(uchar n);
void lcd_xianshi();
void Water_Level();	
void display(uint x);
void measuring();
void init_measuring();


#endif