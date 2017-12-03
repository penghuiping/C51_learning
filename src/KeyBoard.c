#include <reg52.h>

//������̰�ťλ��
int row[4] = {0xFE,0xFD,0xFB,0xF7};	
int column[4] = {0x70,0xB0,0xD0,0xE0};

//��������ֶ�Ӧָ
int arr[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

sbit zhi = P2^0; //��ѡ
sbit wei = P2^2; //λѡ

int value=0;
int timer=0;
int delayTime = 0;

sbit addKey = P0^7;



void digitalTube();

void init_timer0() {
	TMOD = 0x01;//����TMOD,ʹ�ù�����16λ��ʱ
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;
	EA = 1;//�������ж�
	ET0 = 1;//���Ŷ�ʱ��0������ж�
	TR0 = 1;//ʹ��TR0��ʱ
}

void timer0_service(void) interrupt 1
{
	TH0 = (65535-5000) / 256;
	TL0 = (65535-5000) % 256;	
	timer ++;
	if (timer == 1)
	{
		timer = 0;
		//ÿ�봥��һ�εĲ���
		digitalTube();
	}
}

//��ʱ������λms
void delay(int x) {
	int i=0;
	int j=0;
	for(i=x;i>=0;i--)
		for(j=110;j>=0;j--);
	
}


//x��ֵ��n��ָ��
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

//���������
void digitalTube() {
	

		//��̬��ʾ����
		//ע��:���Ͷ�ѡ�źţ�����λѡ��Ϣ����Ϊ����λѡ�źţ�����ܾͿ�ʼ�����ˣ�Ȼ�������Ͷ�ѡ�ͺŵû���������
		zhi=1;
		P1=arr[getWeiData(4,value)];
		zhi=0;
		wei=1;
		P1=(~0x01);
		delay(delayTime);
		P1=0xFF;//��Ӱ��������ѡ�źŵĲ���ֵ��������ܸ��������£�����ֲ�Ӱ
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

//��������
void singleKeyBoard() {
	if(addKey==0) {
			//delay(1);
			if(addKey==0) {
				++value;
				while(!addKey);
			}
		}
}

//�������
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
