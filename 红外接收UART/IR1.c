/*-----------------------------------------
--------通过数码管显示用户码和键值码--------
-----------------------------------------*/

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
		P0=LedChar[ircode[0]>>4];      //取用户码高四位16进制数值
		Delay1ms();
		P2=0xF9;
		P0=LedChar[ircode[0]&0x0F];    //取用户码低四位16进制数值
		Delay1ms();
		P2=0xFC;
		P0=LedChar[ircode[2]>>4];      //取数据码高四位16进制数值
		Delay1ms();
		P2=0xFD;
		P0=LedChar[ircode[2]&0x0F];    //取数据码低四位16进制数值
		Delay1ms();
	}
}
/*-----------------
红外初始化
-----------------*/
void InitInfrared()
{
	IR_INPUT = 1;             //释放红外接收引脚
	TMOD &= 0X0F;             //清零T1控制位
	TMOD |= 0x10;             //配置T1位模式1
	TR1 = 0;                  //关闭T1
	ET1 = 0;                  //关闭T1中断
	IT0 = 1;                  //设置INT0为正边沿触发
	EX0 = 1;                  //使能INT0中断
}
/*----------------------
获取高电平时间
----------------------*/
unsigned int GetHighTime()
{
	TH1 = 0;
	TL1 = 0;
	TR1 = 1;
	while(IR_INPUT)
	{
		if(TH1 > 0xC2)     //当T0计数值大于0xc200时，高电平持续时间大于18ms，强制退出循环
		{
			break;
		}
	}
	TR1 = 0;
	return(TH1 * 256 + TL1);   //返回计数值
}
/*----------------------
获取低电平时间
----------------------*/
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
	//接受判定9ms引导码高电平
	time = GetLowTime();
	if((time <23500) || (time > 26265))	  //判定范围在8.5~9.5ms，如果不符合直接退出
	{
		IE0 = 0;                          //退出时清零中断标志
		return;	
	}
	//接受判定9ms引导码高电平
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
			if((time<940) ||(time >2157))//时间判定超过340~780us误码，直接退出
			{
				IE0 = 0;
				return;
			}
			time = GetHighTime();
			if((time>940) && (time <2157))//时间在340~780us，则该bit值为0
			{
				byt >>= 1;//低位在先，所以时间右移，高位置0
			}
			else if((time>4037) && (time<5253))//时间在1460~1900us，则该bit值为0
			{
				byt >>= 1;
				byt |= 0x80;//低位在先，所以时间右移，高位置1
			}
			else
			{
				IE0 = 0;//否则直接退出
				return;
			}
		}
		ircode[i] = byt;          //接受完一个字节后保存到缓存区
	}
	IE0 = 0;                          //退出时清零中断标志
	irflag=1;                         //设置接收完标志
}
