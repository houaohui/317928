#include <stc12c5a60s2.h>

sbit IR_INPUT = P3^2;
bit irflag = 0;
unsigned char ircode[4];
void delay()
{
	unsigned int i,n;
	for(i=0;i<6000;i++)
	{
		n=600;
		while(n--);
	}
}

void InitInfrared()
{
	 IR_INPUT = 1;
	 TMOD |= 0x01;
	 TR0 = 0;
	 ET0 = 0;
	 IT0 = 1;
	 EX0 = 1;
}

unsigned int GetHighTime()
{
 
	 TH0 = 0;
	 TL0 = 0;
	 TR0 = 1;
	while (IR_INPUT)
	{
			if (TH0 >= 0xc2)
		{
			break;
		}
	}
	 TR0 = 0;
	 return (TH0*256 + TL0);
}

unsigned int GetLowTime()
{
	 TH0 = 0;
	 TL0 = 0;
	 TF0 = 0;
	 TR0 = 1;
	 while (!IR_INPUT)
	 {
		 if (TH0 >= 0xc2)
		 {
				break;
		 }
	 }
	 TR0 = 0;
	 return (TH0*256 + TL0);
}

void EXINT1_ISR() interrupt 0
{
	 unsigned char i, j;
	 unsigned char byt;
	 unsigned int time;
	 

 time = GetLowTime();
 if ((time<23500)||(time>26265))
 {
	 IE0 = 0;
	 return;
 }

 time = GetHighTime();
 if ((time<11059)||(time>13824))
 {
	 IE0 = 0;
	 return;
 }

 for (i=0; i<4; i++)
 {
	 for (j=0; j<8; j++)
	 {
		 time = GetLowTime();
		 if ((time<940) || (time>2157))
		 {
			 IE0 = 0;
			 return;
		 }
		 time = GetHighTime();
		 if ((time>940) && (time<2157))
		 {
				byt >>= 1;
		 }
		 else if ((time>4037) && (time<5253))
		 {
			 byt >>= 1;
			 byt |= 0x80;
		 }
		 else
		 {
			 IE0 = 0;
			 return;
		 }
	 }
	 ircode[i] = byt;
 }
 irflag = 1;
 IE0 = 0;
}
void main()
{
	unsigned char i;
	TMOD |=0x20; //设置定时器1工作模式1，8位自动重装
	TL1=TH1=0xf7; //比特率9600
	SM1=1; //设置串口工作模式1，10位异步收发
	TR1=1; //启动定时器1
	EA=1;
	InitInfrared();
	while(1)
	{
		if(irflag)
		{
				for(i=0;i<4;i++)
				{
					SBUF=ircode[i];
					while(!TI);
					TI=0;
				}
//			delay();
				irflag=0;
		}
	}
}


