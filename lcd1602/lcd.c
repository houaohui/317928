#include <stc12c5a60s2.h>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P1^2;
/*--------------
空函数，为了延时300ns
---------------*/
void delay()
{
}
/*----------------
读忙
---------------*/
void read_busy()
{
	uchar busy;
	P0=0xff;
	RS=0;
	RW=1;
	do
	{
		EN=1;
		busy=P0;
		delay();
		EN=0;
	}while(busy&0x80);           //bit7==1表示液晶正忙
}
/*------------------
向1602写命令函数
-------------------*/
void write_cmd(uchar cmd)
{
	read_busy();
	RS=0;
	RW=0;
	P0=cmd;
	delay();
	EN=1;
	delay();
	EN=0;
	delay();
}
/*---------------------
向1602写数据函数
----------------------*/
void write_dat(uchar dat)
{
	read_busy();
	RS=1;
	RW=0;
	P0=dat;
	delay();
	EN=1;
	delay();
	EN=0;
	delay();
}

void main()
{
	unsigned char a[]={"monday"};
	unsigned char *pa;
	write_cmd(0x38);                      //设置1602显示
	write_cmd(0x0f);                      //开显示，显示光标，光标闪烁
	write_cmd(0x01);                      //清屏
	write_cmd(0x06);                      //地址指针移位命令
	write_cmd(0x80|0x00);                 //显示地址，从第零位开始
	pa=a;
	while(*pa!='\0')
	{
		write_dat(*pa++);             //先取数据再地址加一
	}
	while(1);
}






