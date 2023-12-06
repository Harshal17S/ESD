#include<lpc214x.h>

#define bit(x) (1 << x)
#define delay for(i = 0; i <= 60000; i++)

unsigned int i;

int main() {
    IO0DIR = 0x3; // Declaring P0.0 and P0.1 as output

    while (1) {
        /* Forward */
        IO0SET = bit(0); // IN1 = 1
        IO0CLR = bit(1); // IN2 = 0
        delay;
        delay;

        /* Off */
        IO0CLR = bit(0) | bit(1); // IN1 = IN2 = 0
        delay;
        delay;

        /* Reverse */
        IO0SET = bit(1); // IN2 = 1
        IO0CLR = bit(0); // IN1 = 0
        delay;
        delay;

        /* Off */
        IO0CLR = bit(0) | bit(1); // IN1 = IN2 = 0
        delay;
        delay;
    }
    return 0;
}
