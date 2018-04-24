#include <reg51.h>
#include "lcd12864.h"
#include "ds1302.h"
#include "key.h"

void Int1Configuration();
void LcdDisplay();
uchar SetPlace;
bit SetState=0;
/*******************************************************************************
* ������         : key
* ��������		 	 : ��������ʱ��
* ����           : ��
* ���         	 : ��
*******************************************************************************/

void key()
{
    unsigned char i;
	if(SetState==0)
	{
		Ds1302ReadTime();
	}
	else if(SetState==1)
		{
			if(K1==0)		//��ⰴ��K1�Ƿ���
			{
				Delay10ms(1);	//��������
				if(K1==0)
				{
					SetPlace++;
					if(SetPlace>=7)
						SetPlace=0;					
				}

				while((i<50)&&(K1==0))	 //��ⰴ���Ƿ��ɿ�
				{
					Delay10ms(1);
					i++;
				}
				i=0;
			}
			if(K2==0)		//��ⰴ��K2�Ƿ���
			{
				Delay10ms(1);	//��������
				if(K2==0)
				{
					TIME[SetPlace]++;
					if((TIME[SetPlace]&0x0f)>9)					 //����BCD�롣
					{
						TIME[SetPlace]=TIME[SetPlace]+6;
					}
					if((TIME[SetPlace]>=0x60)&&(SetPlace<2))		//����ֻ�ܵ�59
					{
						TIME[SetPlace]=0;
					}
					if((TIME[SetPlace]>=0x24)&&(SetPlace==2))		//Сʱֻ�ܵ�23
					{
						TIME[SetPlace]=0;
					}
					if((TIME[SetPlace]>=0x32)&&(SetPlace==3))		//��ֻ�ܵ�31
					{
						TIME[SetPlace]=0;	
					}
					if((TIME[SetPlace]>=0x13)&&(SetPlace==4))		//��ֻ�ܵ�12
					{
						TIME[SetPlace]=0;
					}	
					if((TIME[SetPlace]>=0x7)&&(SetPlace==5))		//��ֻ�ܵ�7
					{
						TIME[SetPlace]=1;
					}		
//					if(SetPlace==5)		//��ֻ�ܵ�12
//					{
//						TIME[SetPlace]=;
//					}	
				}
				while((i<50)&&(K2==0))	 //��ⰴ���Ƿ��ɿ�
				{
					Delay10ms(1);
					i++;
				}
				i=0;
		    Ds1302Init();
			}						
		}
}

/*******************************************************************************
* ������         : Int1Configuration()
* ��������			 : �����ⲿ�ж�1
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Int1Init()
{
	//����INT1
	IT1=1;//�����س�����ʽ���½��أ�
	EX1=1;//��INT1���ж�����
	EA=1;//�����ж�	
}

/*******************************************************************************
* ������         : Int1() 
* ��������			 : �ⲿ�ж�1�жϺ���
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Int1() interrupt 2		 
{
	if(K3==0)
	{
		Delay10ms(1);
		if(K3==0)
		{
		SetState=~SetState;
		SetPlace=0;
		Ds1302ReadTime();
		}			
	}
}