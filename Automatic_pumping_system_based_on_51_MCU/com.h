#ifndef __COM_H_
#define __COM_H_

//---����ͷ�ļ�---//
#include<reg51.h>

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---����ȫ�ֺ���---//
void tempshow();
void temp(int);
void UsartConfiguration();

#endif