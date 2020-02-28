/*---------------------------------------------
---按键即是钢琴键，来控制蜂鸣器发出不同频率-----
---可以通过上下键调节频率----------------------
---------------------------------------------*/
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

unsigned char keyscan()
{
	unsigned char cl,ch;
	P2=0x0f;
	if((P2 & 0x0f)!= 0x0f)
	{
		cl= P2 & 0x0f;
		P2=cl|0xf0;
		ch=P2&0xf0;
		while((P2&0xf0)!=0xf0);
		return (cl + ch);
	}
}

void main()
{
	unsigned char a,b,c,d,e,cnt=0;
	unsigned int i=400,n=3100;
	bit en=1;
	TMOD=0x11;
	TH1=0X00;
	TL1=0X00;
	TR1=1;
	EA=1;
	P24=0;
	enled=0;
	while(1)
	{
		
		switch(keyscan())
		{
			case 0x7e:en=1;i=n+400; break;   //1
			case 0x7d:en=1;i=n+800;break; 
			case 0x7b:en=1;i=n+1200;break; 
			
			case 0x77:en=1;n=n+5;break; 
			case 0xd7:en=1;n=n-5;break; 
			
			case 0xbe:en=1;i=n+1600;break; 
			case 0xbd:en=1;i=n+2000;break; 
			case 0xbb:en=1;i=n+2400;break; 
			
			case 0xde:en=1;i=n+2800; break;   //9
			case 0xdd:en=1;i=n+3200;break; 
			case 0xdb:en=1;i=n+3600;break; 
		}
		if(en)
		{
			openbuzz(i);
			en=0;
		}
	
		if(TF1==1)
		{
			TF1=0;
			TH1=0X00;
			TL1=0X00;
			cnt++;
			if(cnt>=10)
			{
				cnt=0;
				stopbuzz();
				buzz=1;
			}
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
	TR1=1;
}
void stopbuzz()
{
	ET0=0;
	TR0=0;
	TR1=0;
}
void interruptT0() interrupt 1
{
	TH0=torh;
	TL0=torl;
	buzz=~buzz;
}
