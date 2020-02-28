#include<stc12c5a60s2.h>

sbit ENLED=P1^1;
sbit KEY_IN_1=P2^0;
sbit KEY_IN_2=P2^1;
sbit KEY_IN_3=P2^2;
sbit KEY_IN_4=P2^3;

unsigned long PeriodCnt=0;
unsigned char HighRH=0;
unsigned char HighRL=0;
unsigned char LowRH=0;
unsigned char LowRL=0;
unsigned char T1RH=0;
unsigned char T1RL=0;
unsigned int index=0;
unsigned char sec=120;
unsigned char flag1s=0;
bit PWMOUT=1;

unsigned char code LedChar[]={
	0xC0,0xF9,0xA4,0xB0,
	0x99,0x92,0x82,0xF8,
	0x80,0x90,0x88,0x83,
	0xC6,0xA1,0x86,0x8E
};
unsigned char str1[]="A";
unsigned char str2[]="B";
unsigned char str3[]="C";
unsigned char LedBuff[7]={
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
unsigned char KeySta[4]={1,1,1,1};

bit KeyStart=0;

extern void InitLcd1602();
extern bit Start18B20();
bit Get18B20Temp(int *temp);
void KeyDirver();
void ConfigTimer1(unsigned int ms);
void AdjustDutyCycle(unsigned char dc);
void ConfiPWM(unsigned int fr,unsigned char dc);
unsigned char IntToString(unsigned char *str,int dat);
extern void LcdShowStr(unsigned char x,unsigned char y,char *str);
extern void LcdAreaclean(unsigned char x,unsigned char y,unsigned char leng);

void main()
{
//	bit res;
	int temp;
	int intT,decT;
	unsigned char len;
	unsigned char str[12];

	ENLED=0;
	PT1=1;
	Start18B20();
	InitLcd1602();
	ConfigTimer1(1);
	ConfiPWM(100,100);
	EA=1;
	while(1)
	{
		KeyDirver();
		if(flag1s)
		{
			flag1s=0;

			if(Get18B20Temp(&temp))
			{
				intT=temp>>4;
				decT=temp&0xF;
				len=IntToString(str,intT);
				str[len++]='.';
				decT=(decT*10)/16;
				str[len++]=decT+'0';
				while(len<6)
				{
					str[len++]=' ';
				}
				str[len]='\0';
				LcdShowStr(0,1,str);
			}
			else
			{
				LcdShowStr(0,1,"error!");
			}
			Start18B20();
		}
	
		if(KeyStart)
		{
			ET0=1;
		}
		else
		{
			ET0=0;
		}
	}
}
unsigned char IntToString(unsigned char *str,int dat)
{
	signed char i=0;
	unsigned char len=0;
	unsigned char buf[6];

	if(dat<0)
	{
		dat=-dat;
		*str++='-';
		len++;
	}
	do{
		buf[i++]=dat%10;
		dat/=10;
	}while(dat>0);
	len+=i;
	while(i-->0)
	{
		*str++=buf[i]+'0';
	}
	*str='\0';
	return len;
}
void ConfigTimer1(unsigned int ms) //����t1�Ķ�ʱʱ��
{
	unsigned long tmp;
	tmp=33177600/12;
	tmp=(tmp*ms)/1000;
	tmp=65536-tmp;
	T1RH=(unsigned char)(tmp>>8);
	T1RL=(unsigned char)tmp;
	TMOD&=0x0F;
	TMOD|=0x10;
	TH1=T1RH;
	TL1=T1RL;
	TR1=1;
	ET1=1; 
}
void ConfiPWM(unsigned int fr,unsigned char dc) //д��pwmƵ�ʺ�ռ���ʲ�����t0��ʵ��pwm
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
void AdjustDutyCycle(unsigned char dc) //���������ı�ռ����
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
void LedReset()	//�رն�ʱ��
{
	KeyStart=0;
	sec=120;	//�����жϹر�pwm��С��
	P13=1;
	LedBuff[5]=0xff;	
}
void mode1()
{
	AdjustDutyCycle(20);
	LedBuff[5]=LedChar[10];	//���丳ֵ�õ������������ʾA
	KeyStart=1;
	LcdShowStr(0,0,str1);
}
void mode2()
{
	AdjustDutyCycle(50);
	LedBuff[5]=LedChar[11]; //���丳ֵ�õ������������ʾB
	KeyStart=1;
	LcdShowStr(0,0,str2);		
}

void mode3()
{
	AdjustDutyCycle(70);	
	LedBuff[5]=LedChar[12];	//���丳ֵ�õ������������ʾC
	KeyStart=1;
	LcdShowStr(0,0,str3);	
}
void KeyDirver() //�԰������н���ѡ��
{
	static unsigned char backup[4]={1,1,1,1};
	unsigned char i;
	for(i=0;i<=3;i++)
	{
		if(backup[i]!=KeySta[i])
			{
				if(backup[i]!=0)	
				{
					if(i==0)
					LedReset();
					else if(i==1)
					mode1();
					else if(i==2)
					mode2();
					else if(i==3)
					mode3();
			 	}
			backup[i]=KeySta[i];
			}
	}
}
void KeyScan()
{
	unsigned char i;
	static unsigned char keybuf[4]={0xFF,0xFF,0xFF,0xFF};
	P2=0x0f;
	
	keybuf[0]=(keybuf[0]<<1)|KEY_IN_1;
	keybuf[1]=(keybuf[1]<<1)|KEY_IN_2;
	keybuf[2]=(keybuf[2]<<1)|KEY_IN_3;
	keybuf[3]=(keybuf[3]<<1)|KEY_IN_4;  
	for(i=0;i<=3;i++)
		{
			if((keybuf[i]&0x0F)==0x00)
			KeySta[i]=0;
			else if((keybuf[i]&0x0F)==0x0F)
			KeySta[i]=1;
		}
}
void ShowNumber(unsigned char num)	 //�����ֽ���ɨ�账��
{
	signed char i;
	unsigned char buf[3];
	for(i=2;i>=0;i--)
		{
			buf[i]=num%10;
			num/=10;
		}
	for(i=2;i>=0;i--)
	{
		LedBuff[i]=LedChar[buf[i]];	 //ѡ��������ʾ
	}	
}
void LedScan() //ɨ������ܺ�С��
{
	static unsigned char i=0;
	P0=0xFF;
	P2=(P2&0xF8)|i;
	P0=LedBuff[i];
	if(i<6)
		i++;
	else
		i=0;
}
void InterruptTimer0() interrupt 1
{
	if(PWMOUT==1)	   //ѡ��pwm�͵�ƽ
		{
			TH0=LowRH;
			TL0=LowRL;
			PWMOUT=0;
			LedBuff[6]=0xff; //�رյ���
			P13=1;  //�رյ��
		}
	else			   //ѡ��pwm�ߵ�ƽ
		{
			TH0=HighRH;
			TL0=HighRL;
			PWMOUT=1;
			LedBuff[6]=0x7f;//�õ�8�����ݵ������̶�������pwm	�򿪵���
			P13=0;  //��p1.6������ֱ������ϵ�
		}
}
void InterruptTimer1() interrupt 3
{
	static unsigned char m;
	TH1=T1RH;
	TL1=T1RL;
	ShowNumber(sec);	
	KeyScan(); 
	LedScan();
	index++;
	m++;
	if(index>=1000)
	{
		sec--;
		index=0;
		flag1s=1;
	}
	if(m>=700&&m<800)
	{
		LedBuff[6]&=0xBF; //���߸�����ÿ800ms����
	}
	if(m>800)
		m=0;
}
