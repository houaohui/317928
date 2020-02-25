#include <stc12c5a60s2.h>
sbit DS = P3^5; //1_wire
sbit enled=P1^1;
unsigned char code ledchar[]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//进入函数0.98us,减一次0.49us
void delay(unsigned int us)
{
	while(us--);
}

//初始化程序
bit ds_init()
{
	bit i;
	DS=1;
	delay(1);
	DS=0;
	delay(998);//延时480us以上，ds18b20将被复位
	DS=1;
	delay(59);//15~60us,等待
	i=DS;
	delay(300);//采样之后等待60~240us
	DS=1;
	delay(1);
	return (i);
}

//写一个字节
void write_byte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DS=0;
		delay(1);
		DS=dat&0x01;
		delay(150);//延时60us以上
		DS=1; //释放总线准备下次数据写入
		delay(1);//1us
		dat >>= 1;
	}
}
//读一个字节
unsigned char read_byte()
{
	unsigned char i,j,dat;
	for(i=0;i<8;i++)
	{
		DS=0;//产生读时序
		delay(1);//1us
		DS=1;//释放中线采样
		delay(1);//1us
		j=DS;
		delay(150);//60us以上
		DS=1;
		delay(1);//1us
		dat=(j<<7)|(dat>>1);
	}
	return(dat);
}

void main()
{
	unsigned int i;
	unsigned char L,M,a,b,c;
	enled=0;
	ds_init();
	write_byte(0xcc);//发送跳越ROM指令
	write_byte(0x4e);//写暂存器指令
	write_byte(0x7f);//报警上限值
	write_byte(0x00);//报警下限值
	write_byte(0x7f);//配置工作在12位模式下
	ds_init();
	write_byte(0xcc);//发送跳越ROM指令
	write_byte(0x48);//发送考培指令，下次上电后DS18B20会按此配置工作，以后不用配置
	while(1)
	{
		ds_init();
		write_byte(0xcc);//发送跳越ROM指令
		write_byte(0x44);//发送温度转换指令
		delay(1528);
		ds_init();
		write_byte(0xcc);//发送跳越ROM指令
		write_byte(0xbe);//读取ds18b20暂存值
		L=read_byte();
		M=read_byte();
		i=M;
		i<<=8;
		i|=L;
		if(M>=0x08) //判断是否为正数
		{
			i=~i+1; //还原负数取反加一
		}
		i=i*0.0625*10+0.5;
		a=ledchar[i%10];
		b=ledchar[i/10%10];
		c=ledchar[i/100%10];
		
		P20=0;P21=0;P22=1;P0=a;delay(100);P0=0xff;
		P20=1;P21=1;P22=0;P0=b+0x80;delay(100);P0=0xff;
		P20=0;P21=1;P22=0;P0=c;delay(100);P0=0xff;
		P20=1;P21=0;P22=1;P0=0xc6;delay(100);P0=0xff;
	}
}






