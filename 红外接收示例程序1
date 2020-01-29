#include<stc12c5a60s2.h>

typedef unsigned char INT8U;
typedef unsigned char uchar;

typedef unsigned int INT16U;
typedef unsigned int uint;

unsigned char IRtime;				//´¢´æ¼ì²âºìÍâ¸ßµÍµçÆ½³ÖÐøÊ±¼ä
uchar IRcord[4];		//´¢´æ½âÂëºóµÄ4¸ö×Ö½ÚÊý¾Ý
unsigned char IRdata[33];		//°üº¬ÆðÊ¼ÂëÔÚÄÚµÄ33Î»Êý¾Ý
bit IRpro_ok;				//½âÂëºóµÄ4¸ö×Ö½ÚÊý¾Ý½ÓÊÜÍê³É±êÖ¾Î»
bit IRok;						//33Î»Êý¾Ý½ÓÊÜÍê³É±êÖ¾

//
void init()
{
	TMOD |=0x02; //ÉèÖÃ¶¨Ê±Æ÷0£¬¹¤×÷Ä£Ê½2£¬8Î»×Ô¶¯ÖØ×°
	TL0=TH0=0; //¶¨Ê±Æ÷0ÒÆ³ýÒ»´ÎÊ±¼äÎª256¸ö»úÆ÷ÖÜÆÚ
	EA=1; //´ò¿ª×ÜÖÐ¶Ï
	ET0=1;
	TR0=1;
	
	IT0=1; //ÉèÖÃÍâ²¿ÖÐ¶Ï0Ìø±äÑØ´¥·¢·½Ê½
	EX0=1; //¿ªÆôÍâ²¿ÖÐ¶Ï0ÖÐ¶Ï
	
	TMOD |=0x20; //ÉèÖÃ¶¨Ê±Æ÷1¹¤×÷Ä£Ê½1£¬8Î»×Ô¶¯ÖØ×°
	TL1=TH1=0xf7; //±ÈÌØÂÊ9600
	SM1=1; //ÉèÖÃ´®¿Ú¹¤×÷Ä£Ê½1£¬10Î»Òì²½ÊÕ·¢
	TR1=1; //Æô¶¯¶¨Ê±Æ÷1
}

void time0() interrupt 1
{
	IRtime++;//7.716us//24/93.33333
}

void int0() interrupt 0
{
	static uchar i;
	static bit startflag;
	if(startflag)
	{
		/*ÅÐ¶ÏÒýµ¼Âë£¬Èç¹ûÊÇÒýµ¼ÂëÔò´ÓÆðÊ¼Âë¿ªÊ¼´æ*/
		if((IRtime<150)&&(IRtime>=100))
		{
			i=0;
		}
		IRdata[i]=IRtime;
		IRtime=0;   //¼ÆÊýÇåÁã
		i++;
		if(i==33)   //±íÃ÷ÒÑ¾­´æÈë33´ÎÂö¿í
		{
			IRok=1;   //Âö¿í¼ì²éÍê³É
			i=0;      //°ÑÂö¿íÇåÁã×¼±¸ÏÂ´Î´æÈë
			startflag=0;
		}
	}
	else
	{
		IRtime=0;
		startflag=1;
	}
}
//°ÑÌáÈ¡µÄ33´ÎÂö¿í½øÐÐ½âÂë NECÐ­Òé
void IRcordpro()
{
	uchar i;//´¦Àí4¸ö×Ö½Ú
	uchar j;//´¦ÀíÒ»¸ö×Ö½ÚµÄ8Î»Êý¾Ý
	uchar k;//´¦Àí33Î»Âö¿í
	k=1;//´ÓµÚÒ»Î»Âö¿í¿ªÊ¼´¦Àí£¬¶ªµôÆðÊ¼Âë
	for(i=0;i<4;i++)
	{
		for(j=0;j<8;j++)
		{ //´óÓÚÊý¾Ý0µÄÂö¿íÅÐ¶¨ÎªÊý¾Ý1
			if(IRdata[k]>18) IRcord[i] |=0x80;
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
