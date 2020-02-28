#include <stc12c5a60s2.h>
#define uchar unsigned char
sbit enled=P1^1;
unsigned char code ledchar[]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
void delay(uchar i)
{
	while(i--);
}
void main()
{
	int sec=999;
	uchar cnt=0,a,b,c,i=0;
	enled=0;
	TMOD=0X01;
	TH0=0X2f;
	TL0=0Xab;
	TR0=1;
	while(1)
	{
		if(TF0==1)
		{
			TF0=0;
			TH0=0X2f;
			TL0=0Xab;
			cnt++;
			if(cnt>=50)
			{	
				cnt=0;
				sec--;
				a=ledchar[sec%10];
				b=ledchar[sec/10%10];
				c=ledchar[sec/100%10];
			}
		}
		P0=0xff;
		switch(i)
		{
			case 0 :P20=1;P21=0;P22=1;P0=a;delay(100);i++;break;
			case 1 :P20=0;P21=0;P22=1;P0=b;delay(100);i++;break;
			case 2 :P20=1;P21=1;P22=0;P0=c;delay(100);i=0;break;
		}
	}	
}


