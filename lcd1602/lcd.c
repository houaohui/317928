#include <stc12c5a60s2.h>

typedef unsigned char uchar;
typedef unsigned int uint;

sbit RS=P2^0;
sbit RW=P2^1;
sbit EN=P1^2;

void delay()//300ns
{}
void read_busy()
{
	uchar busy;
	P0=0xff;
	RS=0;
	RW=1;
	while(0);
	do
	{
		EN=1;
		busy=P0;
		delay();
		EN=0;
	}while(busy&0x80);
}

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
	write_cmd(0x38);//����1602��ʾ
	write_cmd(0x0f);//����ʾ����ʾ��꣬�����˸
	write_cmd(0x01);//����
	write_cmd(0x06);//��ַָ����λ����
	write_cmd(0x80|0x00);//��ʾ��ַ���ӵ���λ��ʼ
	pa=a;
	while(*pa!='\0')
		write_dat(*pa++);
	while(1);
}






