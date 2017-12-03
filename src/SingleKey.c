#include<reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint;

//��������ֶ�Ӧָ
int arr[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

uchar index=0;
uchar timer=0;

void init_timer0();

int main() {
	init_timer0();
	while(1);
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
		if(index==10)index=0;
	    P1=~arr[index++];
	}
}
