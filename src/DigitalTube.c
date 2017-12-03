#include <reg52.h>


int arr[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

sbit zhi = P2^0; //段选
sbit wei = P2^2; //位选



int value=0;

int timer=0;

//延时函数单位ms
void delay(int x) {
	int i=0;
	int j=0;
	for(i=x;i>=0;i--)
		for(j=110;j>=0;j--);
	
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
		
		//计算累加数字
	    value++;
	}
}

//操作数码管
void digitalTube() {
	while(1) {	
	    
	
		//动态显示数据
		//注意:先送段选信号，在送位选信息，因为送完位选信号，数码管就开始工作了，然后你再送段选型号得话会有乱码
		zhi=1;
		P1=arr[value/1000];
		zhi=0;
		P1=0xFF;//消影操作，段选信号的残留值，在数码管高速运作下，会出现残影
		wei=1;
		P1=(~0x01);
		wei=0;
		
		delay(1);
	
		zhi=1;
		P1=arr[value%1000/100];
		zhi=0;
			
		P1=0xFF;
		wei=1;
		P1=(~0x02);
		wei=0;

		delay(1);
	
		zhi=1;
		P1=arr[value%1000%100/10];
	  	zhi=0;
		P1=0xFF;
		wei=1;
		P1=(~0x04);
		wei=0;

		delay(1);

		zhi=1;
		P1=arr[value%1000%100%10];
		zhi=0;
		P1=0xFF;
		wei=1;
		P1=(~0x08);
		wei=0;
	
		delay(1);	
	}
}

void main(void) {
	init_timer0();
	digitalTube();
}
