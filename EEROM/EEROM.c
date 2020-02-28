#include <stc12c5a60s2.h>

#define uchar unsigned char
#define uint unsigned int
#define AT24C02_ADDR 0xa2
sbit SDA=P3^7;
sbit SCL=P3^6;

void delay_5us()
{
	uchar i=450;
	while(i--);
}
void delay(uchar i)
{
	uchar a=120;
	for(i=120;i>1;i--)
	{
		while(a--);
	}
}
/*-----------------
初始化总线
---------------*/
void I2C_init()
{
	SDA=1;
	while(0);
	SCL=1;
	while(0);
}
/*------------------
起始信号
---------------*/
void I2C_START()
{
	SCL=1;
	while(0);
	SDA=1;
	delay_5us();
	SDA=0;
	delay_5us();
}
/*------------------
停止信号
--------------*/
void I2C_STOP()
{
	SDA=0;
	while(0);
	SCL=1;
	delay_5us();
	SDA=1;
	delay_5us();
}
/*-----------------
主机发送应答
-----------------*/
void master_ACK(bit i)
{
	SCL=1;
	while(0);
	if(i)
	{
		SDA=0;
	}
	else
	{
		SDA=1;
	}
	while(0);
	SCL=1;
	delay_5us();
	SCL=0;
	while(0);
	SDA=1;
	while(0);
}
/*---------------
检测应答
----------------*/
bit TEST_ACK()
{
	SCL=1;
	delay_5us();
	if(SDA)
	{
		SCL=0;
		while(0);
		I2C_STOP();
		return(0);
	}
	else
	{
		SCL=0;
		while(0);
		return(1);
	}
}
/*--------------
发送一个字节
----------------*/
void I2C_send_byte(uchar byte)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		SCL=0;
		while(0);
		if(byte&0x80)
		{
			SDA=1;
			while(0);
		}
		else
		{
			SDA=0;
			while(0);
		}
		SCL=1;
		while(0);
		byte<<=1;
	}
	SCL=0;
	while(0);
	SDA=1;
	while(0);
}
/*----------------
接收一个字节
-----------------*/
uchar I2C_read_byte()
{
	uchar dat,i;
	SCL=0;
	while(0);
	SDA=1;
	while(0);
	for(i=0;i<8;i++)
	{
		SCL=1;
		while(0);
		if(SDA)
		{
			dat=dat|0x01;
		}
		else
		{
			dat=dat&0xfe;
		}
		while(0);
		SCL=0;
		while(0);
		if(i<7)
		{
			dat=dat<<1;
		}
	}
	return(dat);
}


/*-------------------
在EEPROM中的一个地址写入一个字节的数据
--------------------*/
bit I2C_transmitdata(uchar ADDR,DAT)
{
	I2C_START();
	I2C_send_byte(AT24C02_ADDR+0);
	if(!TEST_ACK())
	{
		return(0);           //如果没有应答，直接退出
	}
	I2C_send_byte(ADDR);
	if(!TEST_ACK())
	{
		return(0);
	}
	I2C_send_byte(DAT);
	if(!TEST_ACK())
	{
		return(0);
	}
	I2C_STOP();
	return(1);
}
/*-----------------------
从EEPROM中读取一个字节的数据
------------------------*/
uchar I2C_receivedata(uchar ADDR)
{
	uchar DAT;
	I2C_START();
	I2C_send_byte(AT24C02_ADDR+0);
	if(!TEST_ACK())
	{
		return(0);
	}
	I2C_send_byte(ADDR);
	if(!TEST_ACK())
	{
		return(0);
	}
	
	I2C_START();
	I2C_send_byte(AT24C02_ADDR+1);
	if(!TEST_ACK())
	{
		return(0);
	}
	DAT=I2C_read_byte();
	master_ACK(0);
	I2C_STOP();
	return(DAT);
}




void main()
{
	uchar ACK_flag=0,a=0,i,m=0;
	P11=0;                    //使能P0
	P20=0;
	P21=1;
	P21=1;                    //选择led流水灯
	I2C_init();
	
	
	for(i=0;i<8;i++)          //在EEROM中的0到8号储存空间写入0到8
	{
		I2C_transmitdata(i,m);
		m++;
		delay(60000);
	}
	i=7;
	while(1)                  //重复读取0到8存储空间，并通过流水灯的效果展示
	{
		P0=I2C_receivedata(i);
		i--;
		if(i==-1)i=7;
		delay(60000);
		delay(60000);
	}
}



