#include<REGX52.H>
#include<Timer0.c>

#define GPIO_DIG P0
#define GPIO_KEY P1

sbit Buzzer=P2^5;

typedef unsigned char u8;
typedef unsigned int u16;

u8 freq_select;//Ƶ��ѡ������
u16 FreqTable[36]=
{
									63628,63731,63835,63928,64021,64103,64185,64260,64331,64400,64463,64528,//������
									64580,64633,64684,64732,64777,64820,64860,64898,64934,64968,65000,65030,//������
									65058,65085,65110,65134,65157,65178,65198,65217,65235,65252,65268,65283 //������
};//������Ƶ�ʶ��ձ�

void delay(u16 i)//�ӳٺ���
{
	while(i--);
}

KeyDown(void)//�������ɨ�躯��
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

void Timer0_Routine() interrupt 1//��ʱ���жϺ���
{
	/*ȡ��ӦƵ��ֵ����װ��ֵ����ʱ��*/
	TL0 = FreqTable[freq_select]%256;		//���ö�ʱ��ֵ
	TH0 = FreqTable[freq_select]/256;		//���ö�ʱ��ֵ
	Buzzer=!Buzzer;	//��ת������IO��
}

void main()
{
	u8 position,key_down;
	position=1;//��ʼѡ��������
	Timer0Init();//��ʱ����ʼ��
	EA=0;//���ж�
	while(1)//��ѭ��
	{
		key_down=KeyDown();//�������ȡֵ
		if(key_down<0x04)
		{
			if(key_down==0x03 && position!=2)//���������׼��Ҵ�ʱ���ײ�Ϊ������ʱ
			{
				position++;
			}
			else if(key_down==0x02 && position!=0)//���������׼��Ҵ�ʱ���ײ�Ϊ������ʱ
			{
				position--;
			}
			P2=position^0xff;//����״̬����LEDģ��
			switch(position)
			{
				case(0):freq_select=0;break;//������
				case(1):freq_select=12;break;//������
				case(2):freq_select=24;break;//������
			}
			delay(20000);//��ֹ�ж��ٶȹ���
		}
		else
		{
			switch(key_down)//�޸�Ƶ��ѡ������
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
			EA=1;//���ж�
			while(1)//���¾������ʱѭ��
			{
				TR0=1;//������ʱ��
				if(key_down!=KeyDown())//�ɿ��������ʱ�˳�ѭ��
				{
					TR0=0;//�رն�ʱ��
					EA=0;//���ж�
					break;
				}
			}	
		}
	}
}