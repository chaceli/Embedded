#ifndef __IRLCD_H_
#define __IRLCD_H_

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

//---�������ң������������ʹ�õ�IO��---//
sbit IRIN = P3^2;         //���������������
sbit RELAY= P1^4;         //�̵���������
sbit BEEP = P1^5;         //������������
//sbit LED  = P1^0;					//����LED

//---����ȫ�ֺ���---//
void Timer1Init();
void IrInit();
void irlcd();
void IR_IN(void);
void delay(uchar x);  //x*0.14MS
void delay1(int ms);
void beep(void);

//---����ȫ�ֱ���--//
extern uchar flag;

#endif