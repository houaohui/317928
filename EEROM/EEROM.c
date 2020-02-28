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
	uchar a=60000;
	for(i=60000;i>1;i--)
	{
		while(a--);
	}
}

void I2C_init()
{
	SDA=1;
	while(0);
	SCL=1;
	while(0);
}

void I2C_START() //ÆðÊ¼ÐÅºÅ
{
	SCL=1;
	while(0);
	SDA=1;
	delay_5us();
	SDA=0;
	delay_5us();
}

void I2C_STOP()
{
	SDA=0;
	while(0);
	SCL=1;
	delay_5us();
	SDA=1;
	delay_5us();
}

void master_ACK(bit i) //Ö÷»ú·¢ËÍÓ¦´ð
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
/**/

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
			dat=dat|0x01; //
		}
		else
		{
			dat=dat&0xfe; //
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



bit I2C_transmitdata(uchar ADDR,DAT)
{
	I2C_START();
	I2C_send_byte(AT24C02_ADDR+0);
	if(!TEST_ACK())
	{
		//return(0);
	}
	I2C_send_byte(ADDR);
	if(!TEST_ACK())
	{
		//return(0);
	}
	I2C_send_byte(DAT);
	if(!TEST_ACK())
	{
		//return(0);
	}
	I2C_STOP();
	return(1);
}

uchar I2C_receivedata(uchar ADDR)
{
	uchar DAT;
	I2C_START();
	I2C_send_byte(AT24C02_ADDR+0);
	if(!TEST_ACK())
	{
		//return(0);
	}
	I2C_send_byte(ADDR);
	if(!TEST_ACK())
	{
		//return(0);
	}
	
	I2C_START();
	I2C_send_byte(AT24C02_ADDR+1);
	if(!TEST_ACK())
	{
		//return(0);
	}
	DAT=I2C_read_byte();
	master_ACK(0);
	I2C_STOP();
	return(DAT);
}




void main()
{
	uchar ACK_flag=0,a=0,i,m=0;
	delay_5us();
	delay_5us();
	delay_5us();
	P11=0;
	P20=0;
	P21=1;
	P21=1;
	I2C_init();
//	I2C_START();
//	I2C_send_byte(AT24C02_ADDR+0);
//	if(!TEST_ACK())
//	{
//		ACK_flag=1;
//	}
//	I2C_send_byte(8);
//	if(!TEST_ACK())
//	{
//		ACK_flag=1;
//	}
//	I2C_send_byte(0xaa);
//	if(!TEST_ACK())
//	{
//		ACK_flag=1;
//	}
//	I2C_STOP();
	for(i=0;i<8;i++)
	{
	I2C_transmitdata(i,m);
	m++;
	delay(60000);
	}
	i=7;
	while(1)
	{
	P0=I2C_receivedata(i);
		i--;
	if(i==-1)
		i=7;
	delay(60000);
	delay(60000);
	delay(60000);
	delay(60000);
	delay(60000);
	delay(60000);
	}
	
//	I2C_START();
//	I2C_send_byte(AT24C02_ADDR+0);
//	if(!TEST_ACK())
//	{
//		a=1;
//	}
//	I2C_send_byte(1);
//	master_ACK(0);
//	I2C_START();
//	I2C_send_byte(AT24C02_ADDR+1);
//	if(!TEST_ACK())
//	{
//		a=1;
//	}
//	P0=I2C_read_byte();
//	master_ACK(0);
//	I2C_STOP();
//	if(ACK_flag)
//	{
//		P00=0;
//	}
//	if(a)
//	{
//		P07=0;
//	}
	
}



