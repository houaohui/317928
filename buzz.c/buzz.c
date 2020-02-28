#include <stc12c5a60s2.h>
sbit buzz=P1^3;
sbit enled=P1^1;
unsigned char torh=0;
unsigned char torl=0;
unsigned char code ledchar[]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
void openbuzz(unsigned int frequ);
void stopbuzz();
void delay(unsigned int i)
{
	while(i--);
}

void main()
{
	unsigned char a,b,c,d,e,cnt=0;
	unsigned int i=400;
	bit en=0;
	TMOD=0x11;
	TH1=0X2f;
	TL1=0Xab;
	TR1=1;
	EA=1;
	P24=0;
	enled=0;
	while(1)
	{
		if(P20==0)
		{
			delay(60000);
			if(P20==0)
			{
				i=i+25;
				en=1;
			}
		}
		if(P21==0)
		{
			delay(60000);
			if(P21==0)
			{
				i=i-50;
				en=1;
			}
		}
		if(i<255)i=16588800;
		if(i>16588800)i=255;
		if(en)
		{
			openbuzz(i);
			en=0;
		}
		
			a=ledchar[i%10];
			b=ledchar[i/10%10];
			c=ledchar[i/100%10];
			d=ledchar[i/1000%10];
			e=ledchar[i/10000%10];
			
		
			P20=1;P21=0;P22=1;P0=a;delay(100);P0=0xff;//¹Ø±Õ¶ÎÑ¡ÏûÓ°×Ó
			P20=0;P21=0;P22=1;P0=b;delay(100);P0=0xff;
			P20=1;P21=1;P22=0;P0=c;delay(100);P0=0xff;
			P20=0;P21=1;P22=0;P0=d;delay(100);P0=0xff;
		  P20=1;P21=0;P22=0;P0=e;delay(100);P0=0xff;
		  P20=1;P21=1;P22=1;
	}
	
}
void openbuzz(unsigned int frequ)
{
	unsigned int reload;
	reload=65536-(33177600/1)/(frequ*2);
	torh=(unsigned char)(reload>>8);
	torl=(unsigned char)reload;
	ET0=1;
	TR0=1;
}
void stopbuzz()
{
	ET0=0;
	TR0=0;
}
void interruptT0() interrupt 1
{
	TH0=torh;
	TL0=torl;
	buzz=~buzz;
}
