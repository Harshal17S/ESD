#include <lpc214x.h>

void ser_init(void);
void tx(unsigned char c);
unsigned char rx(void);
void tx_string(unsigned char *s);
unsigned char keypad(void);

#define c1 (IOPIN1 & 1 << 20)
#define c2 (IOPIN1 & 1 << 21)
#define c3 (IOPIN1 & 1 << 22)

unsigned char r_loc, c_loc;
unsigned char key[4][3] = {"123", "456", "789", "*0#"};

int main()
{
    IO1DIR = 0x0F << 16;
    ser_init();

    while (1)
    {
        tx_string("Key: ");
        tx(keypad());
    }
}

unsigned char keypad(void)
{
    IO1PIN = 0xF0 << 16;
    while (c1 && c2 && c3)
        ;
    while (!c1 || !c2 || !c3)
    {
        if (!c1 && c2 && c3)
            c_loc = 0;
        else if (c1 && !c2 && c3)
            c_loc = 1;
        else if (c1 && c2 && !c3)
            c_loc = 2;

        IO1CLR = 1 << 16;
        IO1SET = 0x0E << 16;
        if (!c1 || !c2 || !c3)
        {
            r_loc = 0;
            break;
        }

        IO1CLR = 1 << 17;
        IO1SET = 0x0D << 16;
        if (!c1 || !c2 || !c3)
        {
            r_loc = 1;
            break;
        }

        IO1CLR = 1 << 18;
        IO1SET = 0x0B << 16;
        if (!c1 || !c2 || !c3)
        {
            r_loc = 2;
            break;
        }

        IO1CLR = 1 << 19;
        IO1SET = 0x07 << 16;
        if (!c1 || !c2 || !c3)
        {
            r_loc = 3;
            break;
        }
    }

    while (!c1 || !c2 || !c3)
        ;
    return (key[r_loc][c_loc]);
}

void ser_init()
{
    VPBDIV = 0x01; // PCLK = 30MHz
    PINSEL0 = 0x5;
    U0LCR = 0x83;
    U0DLL = 195;
    U0DLM = 0;
    U0LCR = 0x03;
    U0TER = (1 << 7);
}

void tx(unsigned char c)
{
    U0THR = c;
    while ((U0LSR & (1 << 5)) == 0);
}

void tx_string(unsigned char *s)
{
    while (*s)
    {
        tx(*s++);
    }
}

unsigned char rx()
{
    while ((U0LSR & (1 << 0)) == 0);
    return U0RBR;
}
