#include <lpc214x.h>
#include <stdio.h>

#define adc_sel 27
#define clkdiv 8
#define ch0 0
#define burst_mode 16
#define pdn 21
#define start 24
#define done 31
#define lcdport IO0SET
#define lcdportclr IOCLR0
#define rs 12
#define en 13
#define TEMP_PIN (1 << 29)
#define TEMP_PIN_DIR (1 << 26)
#define _PDN_BIT (1 << 21)
#define _ADCR_START_MASK (7 << 24)
#define _ADCR_SEL_MASK 0x000000FF
#define _ADC0_START (1 << 24)
#define FAN_ON IO1SET = (1 << 18)
#define FAN_OFF IO1CLR = (1 << 18)
#define EXAHAUST_ON IO1SET = (1 << 19)
#define EXAHAUST_OFF IO1CLR = (1 << 19)
#define DRAIN_ON IO1SET = (1 << 20)
#define DRAIN_OFF IO1CLR = (1 << 20)

void delay(int t) {
    int i, j;
    for (i = 0; i < t; i++)
        for (j = 0; j < 5000; j++)
            ;
}

void cmnd() {
    lcdportclr = (1 << rs);
    lcdport = (1 << en);
    delay(40);
    lcdportclr = (1 << en);
}

void lcdcmd(char ch) {
    lcdport = ((ch & 0xf0) << 13);
    cmnd();
    lcdportclr = ((ch & 0xf0) << 13);
    lcdport = (((ch << 4) & 0xf0) << 13);
    cmnd();
    lcdportclr = (((ch << 4) & 0xf0) << 13);
}

void daten() {
    lcdport = (1 << rs);
    lcdport = (1 << en);
    delay(40);
    lcdportclr = (1 << en);
}

void lcddata(char ch) {
    lcdport = ((ch & 0xf0) << 13);
    daten();
    lcdportclr = ((ch & 0xf0) << 13);
    lcdport = (((ch << 4) & 0xf0) << 13);
    daten();
    lcdportclr = (((ch << 4) & 0xf0) << 13);
}

void lcdstring(char *str) {
    int j;
    for (j = 0; str[j] != '\0'; j++) {
        lcddata(str[j]);
    }
}

void lcd_init() {
    lcdcmd(0x02);
    lcdcmd(0x28);
    lcdcmd(0x01);
    lcdcmd(0x0e);
}

void io_init() {
    PINSEL0 = 0X000;
    IODIR0 = 0xffffff;
}

void Adc0Init(unsigned char clk) {
    PCONP |= 0x00001000;
    AD0CR = ((unsigned long)(clk + 1) << 8) | _PDN_BIT;
}

unsigned int Adc0Read(unsigned char channel) {
    static unsigned val;
    AD0CR &= ~(_ADCR_START_MASK | _ADCR_SEL_MASK);
    AD0CR |= ((unsigned long)(1) << channel);
    AD0CR |= _ADC0_START;
    while (!(AD0GDR & (0x80000000)))
        ;
    AD0CR &= ~(_ADCR_START_MASK | _ADCR_SEL_MASK);
    val = AD0GDR;
    val = ((val >> 6) & 0x03FF);
    return (val);
}

int main() {
    float temperature = 0.0;
    char result[5];
    PINSEL1 |= TEMP_PIN_DIR;
    IODIR0 &= ~(TEMP_PIN);
    io_init();
    lcd_init();
    lcdcmd(0x84);
    lcdstring("Temperature");
    Adc0Init(10);

    while (1) {
        temperature = ((float)Adc0Read(2) * 3300 / 1023.0) / 10.0; // Calibrate for LM35
        lcdcmd(0xc5);

        // Use snprintf with an integer variable
        int temp_int = (int)temperature;
        snprintf(result, sizeof(result), "%d", temp_int);

        lcdstring(result);
        lcddata(0xDF);
        lcddata('C');
        delay(100);

        if (temperature >= 0 && temperature <= 30) {
            DRAIN_OFF;
            EXAHAUST_OFF;
            FAN_ON;
        } else if (temperature > 30 && temperature <= 50) {
            FAN_OFF;
            DRAIN_OFF;
            EXAHAUST_ON;
        } else if (temperature >= 50 && temperature <= 70) {
            EXAHAUST_OFF;
            DRAIN_ON;
            FAN_OFF;
        }
    }
}