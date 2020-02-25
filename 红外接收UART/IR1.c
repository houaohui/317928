#include <STC12C5A60S2.H>
sbit IR_INPUT = P3^2; 
sbit ENLED = P1^1;
bit irflag = 0;  
unsigned char ircode[4];
unsigned char LedBuff[6] = {  
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
unsigned char  LedChar[]={
	0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
	0x80, 0x90 ,0x88 , 0x83 , 0xc6 , 0xa1 , 0x86 , 0x8e
	};
void InitInfrared(void);
void Delay1ms()		//@33.1776MHz
{
	unsigned char i, j;

	//_nop_();
	//_nop_();
	i = 33;
	j = 66;
	do
	{
		while (--j);
	} while (--i);
}


void main()
{
	EA=1;
	ENLED=0;
	InitInfrared();
	
	while(1)
	{
		if(irflag)
		irflag=0;	
			P2=0X00;
		P0=LedChar[ircode[0]>>4];
			Delay1ms();
			P2=0xF9;
		P0=LedChar[ircode[0]&0x0F];
			Delay1ms();
			P2=0xFC;
		P0=LedChar[ircode[2]>>4];
			Delay1ms();
			P2=0xFD;
		P0=LedChar[ircode[2]&0x0F];
			Delay1ms();
}	
	}
void InitInfrared()
{	
	IR_INPUT = 1;
	TMOD &= 0X0F;
	TMOD |= 0x10;
	TR1 = 0;
	ET1 = 0;
	IT0 = 1;
	EX0 = 1;
}
unsigned int GetHighTime()
{
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;
	while(IR_INPUT)
	{
		if(TH1 > 0xC2)
		{
			break;
		}
	}
	TR1 = 0;

	return(TH1 * 256 + TL1);
}

unsigned int GetLowTime()
{
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;
	while(!IR_INPUT)
	{
		if(TH1 > 0xC2)
		{
			break;
		}
	}
	TR1 = 0;

	return(TH1 * 256 + TL1);
}
void EXINT1_ISR() interrupt 0
{
	unsigned char i, j;
	unsigned int time;
	unsigned char byt;

	time = GetLowTime();
	if((time <23500) || (time > 26265))	   
	{
		IE0 = 0;
		return;	
	}

	time = GetHighTime();
	if((time<11059) || (time > 13824))
	{
		IE0 = 0;
		return;
	}
	for(i=0; i<4; i++)
	{
		for(j=0; j<8; j++)
		{
			time = GetLowTime();
			if((time<940) ||(time >2157))
			{
				IE0 = 0;
				return;
			}
			time = GetHighTime();
			if((time>940) && (time <2157))
			{
				byt >>= 1;	
			}
			else if((time>4037) && (time<5253))
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
	
	IE0 = 0;
	irflag=1;
}


