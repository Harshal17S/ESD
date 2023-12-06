#include<lpc214x.h>
void delay(int );
int i;

unsigned int
	a[]={0x00000040,0x00000079,0x00000024,0x00000030,0x00000019,0x00000012,0x00000002,0x00000078,0x00000000,0x00000010};
	
	void delay(int k)
	{
		int i,j;
		for(i=0;i<k;i++)
		for(j=0;j<=1000;j++);	}
	
	int main()
	{
		IO0DIR=IO0DIR|0xffffffff;
		
		while(1)
		{
			for(i=0;i<=9;i++)
			{
				IO0SET = IO0SET | a[i];
				delay(9000);
				IO0CLR = IO0CLR	| a[i];
			}
		}
		return 0;
}