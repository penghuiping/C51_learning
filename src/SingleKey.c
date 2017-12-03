#include<reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

//数码管数字对应指
int arr[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

uchar index=0;
uchar timer=0;

void init_timer0();

int main() {
	init_timer0();
	while(1);
}

void init_timer0() {
	TMOD = 0x01;//设置TMOD,使得工作在16位定时
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;
	EA = 1;//开放总中断
	ET0 = 1;//开放定时器0的溢出中断
	TR0 = 1;//使用TR0定时
}

void timer0_service(void) interrupt 1
{
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;	
	timer ++;
	if (timer == 20)
	{
		timer = 0;
		//每秒触发一次的操作
		if(index==10)index=0;
	    P1=~arr[index++];
	}
}
