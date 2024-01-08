#include<REGX52.H>
#include<Timer0.c>

#define GPIO_DIG P0
#define GPIO_KEY P1

sbit Buzzer=P2^5;

typedef unsigned char u8;
typedef unsigned int u16;

u8 freq_select;//频率选择索引
u16 FreqTable[36]=
{
									63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,//低音段
									64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,//中音段
									65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283 //高音段
};//索引与频率对照表

void delay(u16 i)//延迟函数
{
	while(i--);
}

KeyDown(void)//矩阵键盘扫描函数
{
	u8 KeyValue=0x00;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)
	{
		delay(1000);
		if(GPIO_KEY!=0x0f)
		{
			GPIO_KEY=0x0f;
			switch(GPIO_KEY)
			{
				case(0x07):KeyValue=0x00;break;
				case(0x0b):KeyValue=0x01;break;
				case(0x0d):KeyValue=0x02;break;
				case(0x0e):KeyValue=0x03;break;
			}
			GPIO_KEY=0xf0;
			switch(GPIO_KEY)
			{
				case(0x70):break;
				case(0xb0):KeyValue+=0x04;break;
				case(0xd0):KeyValue+=0x08;break;
				case(0xe0):KeyValue+=0x0c;break;
			}
		}
	}
	return KeyValue;
}

void Timer0_Routine() interrupt 1//定时器中断函数
{
	/*取对应频率值的重装载值到定时器*/
	TL0 = FreqTable[freq_select]%256;		//设置定时初值
	TH0 = FreqTable[freq_select]/256;		//设置定时初值
	Buzzer=!Buzzer;	//翻转蜂鸣器IO口
}

void main()
{
	u8 position,key_down;
	position=1;//初始选择中音段
	Timer0Init();//定时器初始化
	EA=0;//关中断
	while(1)//主循环
	{
		key_down=KeyDown();//矩阵键盘取值
		if(key_down<0x04)
		{
			if(key_down==0x03 && position!=2)//当按下音阶减且此时音阶不为低音段时
			{
				position++;
			}
			else if(key_down==0x02 && position!=0)//当按下音阶加且此时音阶不为高音段时
			{
				position--;
			}
			P2=position^0xff;//音阶状态送显LED模块
			switch(position)
			{
				case(0):freq_select=0;break;//低音段
				case(1):freq_select=12;break;//中音段
				case(2):freq_select=24;break;//高音段
			}
			delay(20000);//防止判定速度过快
		}
		else
		{
			switch(key_down)//修改频率选择索引
			{
				case(0x04):freq_select+=0x08;break;//Sol#
				case(0x05):freq_select+=0x09;break;//La
				case(0x06):freq_select+=0x0a;break;//La#
				case(0x07):freq_select+=0x0b;break;//Si
				case(0x08):freq_select+=0x04;break;//Mi
				case(0x09):freq_select+=0x05;break;//Fa
				case(0x0a):freq_select+=0x06;break;//Fa#
				case(0x0b):freq_select+=0x07;break;//Sol
				case(0x0c):freq_select+=0x00;break;//Do
				case(0x0d):freq_select+=0x01;break;//Do#
				case(0x0e):freq_select+=0x02;break;//Re
				case(0x0f):freq_select+=0x03;break;//Re#
			}
			EA=1;//开中断
			while(1)//按下矩阵键盘时循环
			{
				TR0=1;//启动定时器
				if(key_down!=KeyDown())//松开矩阵键盘时退出循环
				{
					TR0=0;//关闭定时器
					EA=0;//关中断
					break;
				}
			}	
		}
	}
}