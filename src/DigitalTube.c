#include <reg52.h>


int arr[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

sbit zhi = P2^0; //��ѡ
sbit wei = P2^2; //λѡ



int value=0;

int timer=0;

//��ʱ������λms
void delay(int x) {
	int i=0;
	int j=0;
	for(i=x;i>=0;i--)
		for(j=110;j>=0;j--);
	
}


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
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;	
	timer ++;
	if (timer == 20)
	{
		timer = 0;
		//ÿ�봥��һ�εĲ���
		
		//�����ۼ�����
	    value++;
	}
}

//���������
void digitalTube() {
	while(1) {	
	    
	
		//��̬��ʾ����
		//ע��:���Ͷ�ѡ�źţ�����λѡ��Ϣ����Ϊ����λѡ�źţ�����ܾͿ�ʼ�����ˣ�Ȼ�������Ͷ�ѡ�ͺŵû���������
		zhi=1;
		P1=arr[value/1000];
		zhi=0;
		P1=0xFF;//��Ӱ��������ѡ�źŵĲ���ֵ��������ܸ��������£�����ֲ�Ӱ
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
