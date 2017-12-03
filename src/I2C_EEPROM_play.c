#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char

//数码管数字对应值 0~9
uint arr[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

sbit zhi = P2^0; //段选
sbit wei = P2^2; //位选

sbit sda = P2^7; //i2c数据
sbit scl = P2^6; //i2c时钟

//矩阵键盘按钮位置
uint row[4] = {0xFE,0xFD,0xFB,0xF7};	
uint column[4] = {0x70,0xB0,0xD0,0xE0};

uint value=0;
uint timer=0;
uint timer1=0;
uint delayTime = 0;

sbit addKey = P0^7;

void digitalTube();//显示数码管
void init_timer0() {
	TMOD = 0x01;//设置TMOD,使得工作在16位定时
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;
	EA = 1;//开放总中断
	ET0 = 1;//开放定时器0的溢出中断
	TR0 = 1;//使用TR0定时
}

void init_timer1() {
	TMOD = 0x11;//设置TMOD,使得工作在16位定时
	TH1 = (65535-50000) / 256;
	TL1 = (65535-50000) % 256;
	EA = 1;//开放总中断
	ET1 = 1;//开放定时器1的溢出中断
	TR1 = 1;//使用TR1定时
}


//定时器一，用作显示数码管
void timer0_service(void) interrupt 1
{
	TH0 = (65535-5000) / 256;
	TL0 = (65535-5000) % 256;	
	timer ++;
	if (timer == 1)
	{
		timer = 0;
		//每秒触发一次的操作
		//digitalTube();
	}
}

//定时器一，用按键
void timer1_service(void) interrupt 3
{
	TH1 = (65535-5000) / 256;
	TL1 = (65535-5000) % 256;	
	timer1 ++;
	if (timer1 == 1)
	{
		timer1 = 0;
		//每秒触发一次的操作
		digitalTube();
		
	}
}
//延时函数一个指令周期
void delay(){
	;;
}

//延时函数单位ms
void delay1ms(int x) {
	int i=0;
	int j=0;
	for(i=x;i>=0;i--)
		for(j=110;j>=0;j--);
}


//x是值，n是指数，指数运算
int power(int x,int n) {
	int i = 0;
	int result = 1;
	for(i=0;i<n;i++) {
		result = result*x;
	}
	return result;
}

//获得需要显示的对应位置的数值
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
		delay1ms(delayTime);
		P1=0xFF;//消影操作，段选信号的残留值，在数码管高速运作下，会出现残影
		wei=0;
			
		zhi=1;
		P1=arr[getWeiData(3,value)];
		zhi=0;		
		wei=1;
		P1=(~0x02);
		delay1ms(delayTime);
		P1=0xFF;
		wei=0;
		
		zhi=1;
		P1=arr[getWeiData(2,value)];
	  	zhi=0;
		wei=1;
		P1=(~0x04);
		delay1ms(delayTime);
		P1=0xFF;
		wei=0;
		
		zhi=1;
		P1=arr[getWeiData(1,value)];
		zhi=0;
		wei=1;
		P1=(~0x08);
		delay1ms(delayTime);
		P1=0xFF;
		wei=0;	
}

//i2c初始化
void i2c_init() {
	scl=1;
	delay();
	sda=1;
	delay();
}

//i2c开始通讯
void i2c_start() {
	sda=1;
	delay();
	scl=1;
	delay();
	sda=0;
	delay();
}

//i2c结束通讯
void i2c_stop(){
	sda=0;
	delay();
	scl=1;
	delay();
	sda=1;
	delay();
}

//i2c回应操作
void i2c_respons() {
	uchar i=0;
	scl=1;
	delay();
	while((sda==1)&&(i<255))
		i++;
	scl=0;
	delay();
}

//i2c写操作
void writebyte(uchar c){
	uchar i,temp;
	temp=c;
	scl=0;
	delay();
	for(i=0;i<8;i++){
		temp=temp<<1;
		sda=CY;
		delay();
		scl=1;
		delay();
		scl=0;
		delay();
	}
	sda=1;
	delay();
}

//i2c读操作
uchar readbyte(){
	uchar i,j,k;
	scl=0;
	delay();
	sda=1;
	delay();

	for(i=0;i<8;i++){
		scl=1;
		delay();
		j=sda;
		k=(k<<1)|j;
		scl=0;
		delay();
	}
	sda=1;
	delay();
	return k;
}

//i2c器件写
void i2cwrite(uchar addr,uchar ch){
	i2c_start(); //启动操作
	writebyte(0xa0); //寻址操作，并且告诉此次是写操作
	i2c_respons();//获取器件响应
	writebyte(addr);//告诉eeprom从哪个内部的存储地址开始写数据
	i2c_respons();//获取器件响应
	writebyte(ch);//写一个字节数据
	i2c_respons();//获取器件响应
	i2c_stop();//停止操作
}

//i2c器件读
uchar i2cread(uchar addr){
	uchar temp;
	i2c_start();//启动操作
	writebyte(0xa0);//寻址操作，并且告诉此次是写操作
	i2c_respons();//获取器件响应
	writebyte(addr);//告诉eeprom从哪个内部的存储地址开始读数据
	i2c_respons();//获取器件响应
	i2c_start();//启动操作
	writebyte(0xa1);//寻址操作，并且告诉此次是读操作(必须先写，在切换为读)
	i2c_respons();//获取器件响应
	temp=readbyte();//读一个字节数据
	i2c_stop();//停止操作
	return temp;
}

//矩阵键盘
void matrixKeyBoard() {
	int tmp;
	P0=row[0];//第一行设为低电平
	tmp = P0;
	tmp = tmp & 0xF0;
	if(tmp != 0xF0) {//高四位就是列有低电平
		//delay(1);
		if(tmp != 0xF0) {
			if((tmp & 0xF0)==column[0]) {
				//第一行第一列 重置数据
				value=0;
			}else if(((tmp & 0xF0)==column[1])) {
				//第一行第二列 数据+1
				value++;
			}else if(((tmp & 0xF0)==column[2])) {
				//第一行第三列 写入eeprom
				i2c_init();
				i2cwrite(1,value);
			}else if(((tmp & 0xF0)==column[3])) {
				//第一行第四列 读取eeprom中的数据
				i2c_init();
				value=i2cread(1);
			}
			while(tmp == 0xF0);
		}	
	}
}

void main(void) {
	init_timer0();	
	init_timer1();
	while(1){
		matrixKeyBoard();
	}
}
