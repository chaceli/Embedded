#include "lcd12864.h"
#include <reg51.h>
#include <intrins.h>
#include "shengbo.h"
#include "irlcd.h"

bit flag1;         //触发信号标志位//

uchar count;           //中断累加变量
long int distance;    //测量所得距离

unsigned char code table[ ]={"0123456789"}; //定义字符数组显示数字


/*------------------------------------------------
                  延时函数
------------------------------------------------*/
void delayt(uint x)
{
    uchar j;
    while(x-- > 0)
    {
              for(j = 0;j < 125;j++)
        {
          ;
        }
    }
}


/*------------------------------------------------
定时器0初始化，用于计算响应信号时间
------------------------------------------------*/
void init_t0()
{
    TMOD|=0x01;         
    TL0=0x66;
    TH0=0xfc;              //1ms
    ET0=1; 
		EA=1;
}

/*------------------------------------------------
    超声波模块触发信号
------------------------------------------------*/
void trigger()
{
    trig=0;
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_();
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_(); 
	  	_nop_();
        trig=1;       
}

/*------------------------------------------------
   超声波模块相应端口初始化函数
------------------------------------------------*/
void init_measuring()
{
    trig=1;
    echo=1;
    count=0;
}

/*------------------------------------------------
     超声波模块距离测试函数
------------------------------------------------*/
void measuring()
{
    uchar l;
    uint h,y;
    TR0 = 1;
    while(echo==1)
    {
        ;
    }        
    TR0 = 0;
    l = TL0;
    h = TH0;
    y = (h << 8) + l;
    y = y - 0xfc66;//us部分
    distance = y + 1000 * count;//计算总时间,单位是微秒
    TL0 = 0x66;
    TH0 = 0xfc;
    delayt(30);
    //distance = 3400* distance / 20000;//原始为：（0.34毫米/us）*时间/2//  
		distance = 500* distance / 20000;
}

/*------------------------------------------------
   超声波模块测量结果显示函数
------------------------------------------------*/
void display(uint x)
{
    uchar qian,bai,shi,ge;
    qian=x/1000;
    bai=(x/100)%10;
    shi=(x/10)%10;
    ge=x%10;
    LCD12864_ShowString(1,0,"液面：");
    //LCD12864_SetWindow(1,3);//单位是厘米//
    LCD12864_WriteData(table[qian]);
    LCD12864_WriteData(table[bai]);
    LCD12864_WriteData(table[shi]);
    LCD12864_WriteData('.');
//  LCD12864_WriteData(0x80+0x4d);
    LCD12864_WriteData(table[ge]);
    LCD12864_WriteData('c');
    LCD12864_WriteData('m');
}

//……………………………………………中断服务函数…………………………………………………//

/*------------------------------------------------
              液位输出
------------------------------------------------*/
void Water_Level()            
{   
    trigger();      //触发超声波启动
    while(echo==0)          //等待回声
    {
        ;
    }
    measuring();           //进行距离测量
    display(distance);    //对测量结果进行显示
    init_measuring();    //超声波相应端口初始化

		if(distance>500)
		{
			RELAY = 0;
			flag = 0;
		}
		else
		{
			RELAY = 1;
			flag = 1;
		}
		Delay10ms(200) ;       //每次测量间隔2s
} 
