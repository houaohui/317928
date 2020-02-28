#include<stc12c5a60s2.h>

typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;

unsigned char IRtime;				//储存检测红外高低电平持续时间
uchar IRcord[4];		//储存解码后的4个字节数据
unsigned char IRdata[33];		//包含起始码在内的33位数据
bit IRpro_ok;				//解码后的4个字节数据接受完成标志位
bit IRok;						//33位数据接受完成标志

//
void init()
{
	TMOD |=0x02; //设置定时器0，工作模式2，8位自动重装
	TL0=TH0=0; //定时器0移除一次时间为256个机器周期
	EA=1; //打开总中断
	ET0=1;
	TR0=1;
	
	IT0=1; //设置外部中断0跳变沿触发方式
	EX0=1; //开启外部中断0中断
	
	TMOD |=0x20; //设置定时器1工作模式1，8位自动重装
	TL1=TH1=0xf7; //比特率9600
	SM1=1; //设置串口工作模式1，10位异步收发
	TR1=1; //启动定时器1
}

void time0() interrupt 1
{
	IRtime++;//92.6us
}

void int0() interrupt 0
{
	static uchar i;
	static bit startflag;
	if(startflag)
	{
		/*判断引导码，如果是引导码则从起始码开始存*/
		if((IRtime<202)&&(IRtime>=96))
		{
			i=0;
		}
		IRdata[i]=IRtime;
		IRtime=0;   //计数清零
		i++;
		if(i==33)   //表明已经存入33次脉宽
		{
			IRok=1;   //脉宽检查完成
			i=0;      //把脉宽清零准备下次存入
//			startflag=0;
		}
	}
	else
	{
		IRtime=0;
		startflag=1;
	}
}
//把提取的33次脉宽进行解码 NEC协议
void IRcordpro()
{
	uchar i;//处理4个字节
	uchar j;//处理一个字节的8位数据
	uchar k;//处理33位脉宽
	k=1;//从第一位脉宽开始处理，丢掉起始码
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{ //大于数据0的脉宽判定为数据1
			if(IRdata[k]>15) IRcord[i] |=0x80;
			if(j<7) IRcord[i]>>=1;
			k++;
		}
	}
	IRpro_ok=1;
}

void main()
{
	uchar i;
	init();
	while(1)
	{
		if(IRok)
		{
			IRcordpro();
			IRok=0;
			if(IRpro_ok)
			{
				for(i=0;i<4;i++)
				{
					SBUF=IRcord[i];
					while(!TI);
					TI=0;
				}
				IRpro_ok=0;
				
			}
		}
	}
}



