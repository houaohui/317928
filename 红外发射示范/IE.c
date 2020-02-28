#include <stc12c5a60s2.h>
sbit IEm = P1^3;

void delay_8()		//@33.1776MHz 8.8us
{
	unsigned char i;
	while(0);
	i = 70;
	while (--i);
}
void send(unsigned int i)
{
	while(i--)
	{
		IEm=0;
		delay_8();
		IEm=1;
		delay_8();
		delay_8();
	}
}
void no_send(unsigned int i)
{
	while(i--)
	{
		IEm=1;
		delay_8();
		delay_8();
		delay_8();
	}
}
void send_0()
{
	send(21);
	no_send(21);
}
void send_1()
{
	send(21);
	no_send(64);
}
void send_message(unsigned int user,unsigned char dat)
{
	unsigned char i;
	unsigned int user2;
	user2=user;
	send(342);
	no_send(171);
	for(i=0;i<16;i++)
	{
		if(user2&0x0001)
		{
			send_1();
		}
		else
			send_0();
		user2>>=1;
	}
	user2=dat;
	for(i=0;i<8;i++)
	{
		if(user2&0x01)
		{
			send_1();
		}
		else
			send_0();
		user2>>=1;
	}
	user2=(~dat);
	for(i=0;i<8;i++)
	{
		if(user2&0x01)
		{
			send_1();
		}
		else
			send_0();
		user2>>=1;
	}
	send_0();
}
unsigned char keyscan()
{
	unsigned char cl,ch;
	P2=0x0f;
	if((P2 & 0x0f)!= 0x0f)
	{
			cl= P2 & 0x0f;
			P2=cl|0xf0;
			ch=P2&0xf0;
		  while((P2&0xf0)!=0xf0);
			return (cl + ch);
	}
}
void main()
{
	IEm=0;
	while(1)
	{
		
		switch(keyscan())
		{
			case 0x7e:send_message(0x00bf,0x00); break;
			case 0x7d:send_message(0xb34c,0x93);break;
			case 0x7b:send_message(0xb34c,0xcc);break;
			
//			case 0x77:send_message(0xb34c,0x92);break;
//			case 0xd7:send_message(0xb34c,0x92);break;
			
			case 0xbe:send_message(0xb34c,0x8e);break;
			case 0xbd:send_message(0xb34c,0x8f);break;
			case 0xbb:send_message(0xb34c,0xc8);break;
			
			case 0xde:send_message(0xb34c,0x8a);break;
			case 0xdd:send_message(0xb34c,0x8b);break;
			case 0xdb:send_message(0xb34c,0xc4);break;
		}
	}
	
}
	
