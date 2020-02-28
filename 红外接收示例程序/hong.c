#include<stc12c5a60s2.h>

typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;

unsigned char IRtime;				//���������ߵ͵�ƽ����ʱ��
uchar IRcord[4];		//���������4���ֽ�����
unsigned char IRdata[33];		//������ʼ�����ڵ�33λ����
bit IRpro_ok;				//������4���ֽ����ݽ�����ɱ�־λ
bit IRok;						//33λ���ݽ�����ɱ�־

//
void init()
{
	TMOD |=0x02; //���ö�ʱ��0������ģʽ2��8λ�Զ���װ
	TL0=TH0=0; //��ʱ��0�Ƴ�һ��ʱ��Ϊ256����������
	EA=1; //�����ж�
	ET0=1;
	TR0=1;
	
	IT0=1; //�����ⲿ�ж�0�����ش�����ʽ
	EX0=1; //�����ⲿ�ж�0�ж�
	
	TMOD |=0x20; //���ö�ʱ��1����ģʽ1��8λ�Զ���װ
	TL1=TH1=0xf7; //������9600
	SM1=1; //���ô��ڹ���ģʽ1��10λ�첽�շ�
	TR1=1; //������ʱ��1
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
		/*�ж������룬����������������ʼ�뿪ʼ��*/
		if((IRtime<202)&&(IRtime>=96))
		{
			i=0;
		}
		IRdata[i]=IRtime;
		IRtime=0;   //��������
		i++;
		if(i==33)   //�����Ѿ�����33������
		{
			IRok=1;   //���������
			i=0;      //����������׼���´δ���
//			startflag=0;
		}
	}
	else
	{
		IRtime=0;
		startflag=1;
	}
}
//����ȡ��33��������н��� NECЭ��
void IRcordpro()
{
	uchar i;//����4���ֽ�
	uchar j;//����һ���ֽڵ�8λ����
	uchar k;//����33λ����
	k=1;//�ӵ�һλ����ʼ����������ʼ��
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{ //��������0�������ж�Ϊ����1
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



