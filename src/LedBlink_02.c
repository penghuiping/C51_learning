#include <reg51.h>
typedef unsigned char u8;
typedef unsigned int  u16;
u8 timer;
u8 count;

void initTimer0();

void main (void)
{
	P1=~(0x01<<1);
	while(1);
}

void initTimer0() {
	TMOD = 0x01;
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}


void timer0_service(void) interrupt 1
{
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;	
	timer ++;
	if (timer == 20)
	{
		timer = 0;
		if(count==8)count=0;
		P1=~(0x01<<count++);
	}
}