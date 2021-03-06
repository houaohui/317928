/*--------------------------------------------
-----------小项目-----------------------------
---可设置定时时间，时间为零，蜂鸣器打开--------
--------------------------------------------*/
#include <stc12c5a60s2.h>
#define uchar unsigned char
#define uint unsigned int
sbit buzz=P1^3;
sbit enled=P1^1;
unsigned int sec,a,b,c,s=0,x=9,y=9,z=9;
unsigned char torh=0;
unsigned char torl=0;
unsigned char code ledchar[]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void openbuzz(unsigned int frequ);

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
	return(0);
}


void scan(uchar k)
{
	if(s==1)
	{
		z=k;
	}
	if(s==2)
	{
		y=k;
	}
	if(s==3)
	{
		x=k;
	}
}

void main()
{
	uchar en=1;
	enled=0;
	TMOD=0x11;
	EA=1;
	
	while(1)
	{
		switch(keyscan())
		{
			case 0xee:scan(0);s=0;break;
			case 0x7e:scan(1);s=0;break;   														 //1
			case 0x7d:scan(2);s=0;break; 
			case 0x7b:scan(3);s=0;break; 
			case 0xbe:scan(4);s=0;break; 
			case 0xbd:scan(5);s=0;break; 
			case 0xbb:scan(6);s=0;break; 
			case 0xde:scan(7);s=0;break;  
			case 0xdd:scan(8);s=0;break; 
			case 0xdb:scan(9);s=0;break;		                           //9
			case 0x77:z++;break;                                               //up
			case 0xd7:z--;break;                                               //down
			case 0xb7:s++;if(s>=4)s=1;break; 			           //left
			case 0xe7:s--;if(s<=0)s=3;break;			           //right
			case 0xed:ET0=0;en=1;s=0;ET1=0;TR1=0;break;                        //ESC
			case 0xeb:en=0;ET0=1;TR0=1;break;                                  //ENTER
		}
		if(en)
		{
			if(s==1)
			{
				a=0xff;
			}
			else a=ledchar[z];
			if(s==2)
			{
				b=0xff;
			}
			else b=ledchar[y];
			if(s==3)
			{
				c=0xff;
			}
			else c=ledchar[x];
			sec=x*100+y*10+z;
			x=sec/100%10;
			y=sec/10%10;
			z=sec%10;
			
			P20=1;P21=0;P22=1;P0=a;delay(10);P0=0xff;
			P20=0;P21=0;P22=1;P0=b;delay(10);P0=0xff;
			P20=1;P21=1;P22=0;P0=c;delay(10);P0=0xff;
			P20=1;P21=1;P22=1;
		}
		if(sec==0)
		{
			openbuzz(6666);
		}
	}
}
	
void openbuzz(unsigned int frequ)
{
	unsigned int reload;
	reload=65536-(33177600/1)/(frequ*2);
	torh=(unsigned char)(reload>>8);
	torl=(unsigned char)reload;
	ET1=1;
	TR1=1;
	ET0=0;
}

void interruptT1() interrupt 1
{
	static cnt=0;
	TH0=0xfe;
	TL0=0xb4;
	cnt++;
	if(cnt>=1000)
	{
		cnt=0;
		sec--;
	}
	x=sec/100%10;
	y=sec/10%10;
	z=sec%10;
	a=ledchar[z];
	b=ledchar[y];
	c=ledchar[x];
	
 	P20=1;P21=0;P22=1;P0=a;delay(10);P0=0xff;
	P20=0;P21=0;P22=1;P0=b;delay(10);P0=0xff;
	P20=1;P21=1;P22=0;P0=c;delay(10);P0=0xff;
	P20=1;P21=1;P22=1;
}

void interruptT0() interrupt 3
{
	TH1=torh;
	TL1=torl;
	buzz=~buzz;
}
