#include <stc12c5a60s2.h>
sbit DS = P3^5; //1_wire
sbit enled=P1^1;
unsigned char code ledchar[]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
//���뺯��0.98us,��һ��0.49us
void delay(unsigned int us)
{
	while(us--);
}

//��ʼ������
bit ds_init()
{
	bit i;
	DS=1;
	delay(1);
	DS=0;
	delay(998);//��ʱ480us���ϣ�ds18b20������λ
	DS=1;
	delay(59);//15~60us,�ȴ�
	i=DS;
	delay(300);//����֮��ȴ�60~240us
	DS=1;
	delay(1);
	return (i);
}

//дһ���ֽ�
void write_byte(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DS=0;
		delay(1);
		DS=dat&0x01;
		delay(150);//��ʱ60us����
		DS=1; //�ͷ�����׼���´�����д��
		delay(1);//1us
		dat >>= 1;
	}
}
//��һ���ֽ�
unsigned char read_byte()
{
	unsigned char i,j,dat;
	for(i=0;i<8;i++)
	{
		DS=0;//������ʱ��
		delay(1);//1us
		DS=1;//�ͷ����߲���
		delay(1);//1us
		j=DS;
		delay(150);//60us����
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
	write_byte(0xcc);//������ԽROMָ��
	write_byte(0x4e);//д�ݴ���ָ��
	write_byte(0x7f);//��������ֵ
	write_byte(0x00);//��������ֵ
	write_byte(0x7f);//���ù�����12λģʽ��
	ds_init();
	write_byte(0xcc);//������ԽROMָ��
	write_byte(0x48);//���Ϳ���ָ��´��ϵ��DS18B20�ᰴ�����ù������Ժ�������
	while(1)
	{
		ds_init();
		write_byte(0xcc);//������ԽROMָ��
		write_byte(0x44);//�����¶�ת��ָ��
		delay(1528);
		ds_init();
		write_byte(0xcc);//������ԽROMָ��
		write_byte(0xbe);//��ȡds18b20�ݴ�ֵ
		L=read_byte();
		M=read_byte();
		i=M;
		i<<=8;
		i|=L;
		if(M>=0x08) //�ж��Ƿ�Ϊ����
		{
			i=~i+1; //��ԭ����ȡ����һ
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






