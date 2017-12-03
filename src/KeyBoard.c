#include <reg52.h>

//矩阵键盘按钮位置
int row[4] = {0xFE,0xFD,0xFB,0xF7};	
int column[4] = {0x70,0xB0,0xD0,0xE0};

//数码管数字对应指
int arr[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

sbit zhi = P2^0; //段选
sbit wei = P2^2; //位选

int value=0;
int timer=0;
int delayTime = 0;

sbit addKey = P0^7;



void digitalTube();

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
	TH0 = (65535-5000) / 256;
	TL0 = (65535-5000) % 256;	
	timer ++;
	if (timer == 1)
	{
		timer = 0;
		//每秒触发一次的操作
		digitalTube();
	}
}

//延时函数单位ms
void delay(int x) {
	int i=0;
	int j=0;
	for(i=x;i>=0;i--)
		for(j=110;j>=0;j--);
	
}


//x是值，n是指数
int power(int x,int n) {
	int i = 0;
	int result = 1;
	for(i=0;i<n;i++) {
		result = result*x;
	}
	return result;
}


int getWeiData(int wei,int value) {
	int maxCount = 8+1-wei;
	int count = 0;
	int result = value;
	while(1) {
		++count;
		if(count==maxCount) {
			result = result/power(10,8-count);
			break;
		}else {
			result = result%power(10,8-count);
		}
	}
	return result;
}

//操作数码管
void digitalTube() {
	

		//动态显示数据
		//注意:先送段选信号，在送位选信息，因为送完位选信号，数码管就开始工作了，然后你再送段选型号得话会有乱码
		zhi=1;
		P1=arr[getWeiData(4,value)];
		zhi=0;
		wei=1;
		P1=(~0x01);
		delay(delayTime);
		P1=0xFF;//消影操作，段选信号的残留值，在数码管高速运作下，会出现残影
		wei=0;
			
		zhi=1;
		P1=arr[getWeiData(3,value)];
		zhi=0;		
		wei=1;
		P1=(~0x02);
		delay(delayTime);
		P1=0xFF;
		wei=0;
		
		zhi=1;
		P1=arr[getWeiData(2,value)];
	  	zhi=0;
		wei=1;
		P1=(~0x04);
		delay(delayTime);
		P1=0xFF;
		wei=0;
		
		zhi=1;
		P1=arr[getWeiData(1,value)];
		zhi=0;
		wei=1;
		P1=(~0x08);
		delay(delayTime);
		P1=0xFF;
		wei=0;	
}

//独立按键
void singleKeyBoard() {
	if(addKey==0) {
			//delay(1);
			if(addKey==0) {
				++value;
				while(!addKey);
			}
		}
}

//矩阵键盘
void matrixKeyBoard() {
	int tmp;
	P0=row[0];
	tmp = P0;
	tmp = tmp & 0xF0;
	if(tmp != 0xF0) {
		//delay(1);
		
		if(tmp != 0xF0) {
			if((tmp & 0xF0)==column[0]) {
				value++;
			}
			while(tmp == 0xF0);
		}	
	}
}

void main(void) {
	init_timer0();	
	while(1) {
		matrixKeyBoard();
	}
}
