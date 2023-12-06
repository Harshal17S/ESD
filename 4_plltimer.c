#include<lpc214x.h>
void pllinit()
{
  PLL0CFG =0x24;
	PLL0CON = 0x01;
	PLL0FEED = 0xAA; PLL0FEED = 0x55;
	while((PLL0STAT & 0x400)==0);
	PLL0CON = 0x03;
	PLL0FEED = 0xAA ; PLL0FEED = 0x55;
	VPBDIV = 0x01;
}
void delaymillisec(unsigned int mils)
{
	T0CTCR = 0x00;
	T0PR = 60000 -1;
	T0TC =0;
	T0TCR = 0x01;
	while(T0TC < mils);
	T0TCR = 0x00;
}
void delaymicrosec(unsigned int mics)
{
	T0CTCR = 0x00;
	T0PR = 60 -1;
	T0TC =0;
	T0TCR = 0x01;
	while(T0TC < mics);
	T0TCR = 0x00;
}
	
int main()
{
	pllinit();
	IODIR0=IODIR0 | (0x01 << 16);
	while(1)
	{
		IOSET0=IOSET0 | (0x01 << 16);
		delaymicrosec(1000);
		IOCLR0=IOCLR0 | (0x01 << 16);
		delaymicrosec(1000);
	}	
}