#include <reg51.h>

void delay(int num) {
	int i;
	int j;
	for(j=num;j>0;j--)
		for(i=110;i>0;i--);
}

void main()
{
	while(1) {
 		P2=0x02;
		delay(100);
		P2=0x00;
		delay(100);
	}
}