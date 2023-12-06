#include <LPC214x.h>

void delay()
{
	int i;
	for(i=0; i<3000000; i++);
}

int main()
{
	int data=0x00000000 ;
	
	PINSEL2 = 0X00000000;
	IODIR1 = 0XFF000000;
	
	delay();
	
	while(1)
	{
		// ON and OFF all LED
		IOSET1 = 0XFF000000;
		delay();
		
		IOCLR1 = 0XFF000000;
		delay();
		
		// ON and OFF 4 LED at a time
		IOSET1 = 0XF0000000;
		delay();
		
		IOCLR1 = 0XF0000000;
		delay();
		
		IOSET1 = 0X0F000000;
		delay();
		
		IOCLR1 = 0XFF000000;

		// 1 LED at a time Sequentially
		for(int j =23; j<=31; j++){
			IOSET1 = (1<<j);
			delay();
			IOCLR1 = (1<<j);
			delay();
		}
	}
}