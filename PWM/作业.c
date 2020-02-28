#include<stc12c5a60s2.h>

sbit ENLED=P1^1;
unsigned long PeriodCnt=0;
unsigned char HighRH=0;
unsigned char HighRL=0;
unsigned char LowRH=0;
unsigned char LowRL=0;
bit PWMOUT=1；
/*----------------------------------------
写入pwm频率和占空率并且用t0来实现pwm
----------------------------------------*/
void ConfiPWM(unsigned int fr,unsigned char dc) 
{
	unsigned int high,low;
	PeriodCnt=(33177600/12)/fr;
	high=(PeriodCnt*dc)/100;
	low=PeriodCnt-high;
	high=65536-high;
	low=65536-low;
	HighRH=(unsigned char)(high>>8);
	HighRL=(unsigned char)high;
	LowRH=(unsigned char)(low>>8);
	LowRL=(unsigned char)low;
	TMOD&=0xF0;
	TMOD|=0x01;
	TH0=HighRH;
	TL0=HighRL;
	ET0=1;
	TR0=1;
	PWMOUT=1;
}
/*----------------------------
仅仅用来改变占空率
----------------------------*/
void AdjustDutyCycle(unsigned char dc)
{
	unsigned int high,low;
	high=(PeriodCnt*dc)/100;
	low=PeriodCnt-high;
	high=65536-high+12;
	low=65536-low+12;
	HighRH=(unsigned char)(high>>8);
	HighRL=(unsigned char)high;
	LowRH=(unsigned char)(low>>8);
	LowRL=(unsigned char)low;
}




void main(void)
{
	EA=1;
	ConfiPWM(100,50);
	AdjustDutyCycle(50);
	whille(1);
}

void InterruptTimer0() interrupt 1
{
	if(PWMOUT==1)	           //选择pwm低电平
		{
			TH0=LowRH;
			TL0=LowRL;
			PWMOUT=0;
			P13=1;
		}
	else			   //选择pwm高电平
		{
			TH0=HighRH;
			TL0=HighRL;
			PWMOUT=1;
			P13=0;  
		}
}
