#include<lpc214x.h>
void initpwm(void)
{
	  PINSEL0 = 0x8002;
		PWMPCR = 0x600;
		PWMMR0 = 150000;
		PWMTCR = 0x09;
		PWMMCR = 0x02;
	
	

}
int main(void)
{
 	initpwm();
	while(1)
	{
		PWMMR1 = 75000;
		PWMMR2 = 90000;
		PWMLER = 0x6;
		
	}
	
}	