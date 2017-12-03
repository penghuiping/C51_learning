#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char

//��������ֶ�Ӧֵ 0~9
uint arr[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

sbit zhi = P2^0; //��ѡ
sbit wei = P2^2; //λѡ

sbit sda = P2^7; //i2c����
sbit scl = P2^6; //i2cʱ��

//������̰�ťλ��
uint row[4] = {0xFE,0xFD,0xFB,0xF7};	
uint column[4] = {0x70,0xB0,0xD0,0xE0};

uint value=0;
uint timer=0;
uint timer1=0;
uint delayTime = 0;

sbit addKey = P0^7;

void digitalTube();//��ʾ�����
void init_timer0() {
	TMOD = 0x01;//����TMOD,ʹ�ù�����16λ��ʱ
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;
	EA = 1;//�������ж�
	ET0 = 1;//���Ŷ�ʱ��0������ж�
	TR0 = 1;//ʹ��TR0��ʱ
}

void init_timer1() {
	TMOD = 0x11;//����TMOD,ʹ�ù�����16λ��ʱ
	TH1 = (65535-50000) / 256;
	TL1 = (65535-50000) % 256;
	EA = 1;//�������ж�
	ET1 = 1;//���Ŷ�ʱ��1������ж�
	TR1 = 1;//ʹ��TR1��ʱ
}


//��ʱ��һ��������ʾ�����
void timer0_service(void) interrupt 1
{
	TH0 = (65535-5000) / 256;
	TL0 = (65535-5000) % 256;	
	timer ++;
	if (timer == 1)
	{
		timer = 0;
		//ÿ�봥��һ�εĲ���
		//digitalTube();
	}
}

//��ʱ��һ���ð���
void timer1_service(void) interrupt 3
{
	TH1 = (65535-5000) / 256;
	TL1 = (65535-5000) % 256;	
	timer1 ++;
	if (timer1 == 1)
	{
		timer1 = 0;
		//ÿ�봥��һ�εĲ���
		digitalTube();
		
	}
}
//��ʱ����һ��ָ������
void delay(){
	;;
}

//��ʱ������λms
void delay1ms(int x) {
	int i=0;
	int j=0;
	for(i=x;i>=0;i--)
		for(j=110;j>=0;j--);
}


//x��ֵ��n��ָ����ָ������
int power(int x,int n) {
	int i = 0;
	int result = 1;
	for(i=0;i<n;i++) {
		result = result*x;
	}
	return result;
}

//�����Ҫ��ʾ�Ķ�Ӧλ�õ���ֵ
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

//���������
void digitalTube() {
		//��̬��ʾ����
		//ע��:���Ͷ�ѡ�źţ�����λѡ��Ϣ����Ϊ����λѡ�źţ�����ܾͿ�ʼ�����ˣ�Ȼ�������Ͷ�ѡ�ͺŵû���������
		zhi=1;
		P1=arr[getWeiData(4,value)];
		zhi=0;
		wei=1;
		P1=(~0x01);
		delay1ms(delayTime);
		P1=0xFF;//��Ӱ��������ѡ�źŵĲ���ֵ��������ܸ��������£�����ֲ�Ӱ
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

//i2c��ʼ��
void i2c_init() {
	scl=1;
	delay();
	sda=1;
	delay();
}

//i2c��ʼͨѶ
void i2c_start() {
	sda=1;
	delay();
	scl=1;
	delay();
	sda=0;
	delay();
}

//i2c����ͨѶ
void i2c_stop(){
	sda=0;
	delay();
	scl=1;
	delay();
	sda=1;
	delay();
}

//i2c��Ӧ����
void i2c_respons() {
	uchar i=0;
	scl=1;
	delay();
	while((sda==1)&&(i<255))
		i++;
	scl=0;
	delay();
}

//i2cд����
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

//i2c������
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

//i2c����д
void i2cwrite(uchar addr,uchar ch){
	i2c_start(); //��������
	writebyte(0xa0); //Ѱַ���������Ҹ��ߴ˴���д����
	i2c_respons();//��ȡ������Ӧ
	writebyte(addr);//����eeprom���ĸ��ڲ��Ĵ洢��ַ��ʼд����
	i2c_respons();//��ȡ������Ӧ
	writebyte(ch);//дһ���ֽ�����
	i2c_respons();//��ȡ������Ӧ
	i2c_stop();//ֹͣ����
}

//i2c������
uchar i2cread(uchar addr){
	uchar temp;
	i2c_start();//��������
	writebyte(0xa0);//Ѱַ���������Ҹ��ߴ˴���д����
	i2c_respons();//��ȡ������Ӧ
	writebyte(addr);//����eeprom���ĸ��ڲ��Ĵ洢��ַ��ʼ������
	i2c_respons();//��ȡ������Ӧ
	i2c_start();//��������
	writebyte(0xa1);//Ѱַ���������Ҹ��ߴ˴��Ƕ�����(������д�����л�Ϊ��)
	i2c_respons();//��ȡ������Ӧ
	temp=readbyte();//��һ���ֽ�����
	i2c_stop();//ֹͣ����
	return temp;
}

//�������
void matrixKeyBoard() {
	int tmp;
	P0=row[0];//��һ����Ϊ�͵�ƽ
	tmp = P0;
	tmp = tmp & 0xF0;
	if(tmp != 0xF0) {//����λ�������е͵�ƽ
		//delay(1);
		if(tmp != 0xF0) {
			if((tmp & 0xF0)==column[0]) {
				//��һ�е�һ�� ��������
				value=0;
			}else if(((tmp & 0xF0)==column[1])) {
				//��һ�еڶ��� ����+1
				value++;
			}else if(((tmp & 0xF0)==column[2])) {
				//��һ�е����� д��eeprom
				i2c_init();
				i2cwrite(1,value);
			}else if(((tmp & 0xF0)==column[3])) {
				//��һ�е����� ��ȡeeprom�е�����
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
