//PORT A
#define PORTA_CONF TRISA
#define PORTA_WRITE LATA
#define PORTA_READ PORTA

//PORT B
#define PORTB_CONF TRISB
#define PORTB_WRITE LATB
#define PORTB_READ PORTB

//PORT C
#define PORTC_CONF TRISC
#define PORTC_WRITE LATC
#define PORTC_READ PORTC

//PORT D
#define PORTD_CONF TRISD
#define PORTD_WRITE LATD
#define PORTD_READ PORTD

#define LED_BUILTIN LATAbits.LATA4
#define buttonA PORTCbits.RC4
#define buttonB PORTCbits.RC5
#define botonReset PORTEbits.RE3

// LEDS
#define LED0 LATBbits.LATB7
#define LED1 LATBbits.LATB6
#define LED2 LATBbits.LATB5
#define LED3 LATBbits.LATB4
#define LED4 LATBbits.LATB2
#define LED5 LATBbits.LATB1
#define LED6 LATBbits.LATB0

#define BUZZER_CONF TRISEbits.TRISE0
#define BUZZER_WRITE LATEbits.LATE0

//Definir Puertos
#define _PB0 0
#define _PB1 1
#define _PB2 2
#define _PB3 3
#define _PB4 4
#define _PB5 5
#define _PB6 6
#define _PB7 7

#define _PD0 8
#define _PD1 9
#define _PD2 10
#define _PD3 11
#define _PD4 12
#define _PD5 13
#define _PD6 14
#define _PD7 15

#define _PC0 16
#define _PC1 17
#define _PC2 18
#define _PC6 19
#define _PC7 20



#define _PE0 21
#define _PE1 22
#define _PE2 23

//Entradas analógicas
#define _AN0 0
#define _AN1 1
#define _AN2 2
#define _AN3 3
#define _AN4 4


#define OUTPUT 0
#define INPUT 1


// Variables
bool buttonAflag;
unsigned int buttonApressedcounter;
unsigned int buttonApressedcounter2;
bool buttonBflag;
unsigned int buttonBpressedcounter;
unsigned int buttonBpressedcounter2;
char digitcounter = 0;
char units = 0;
char tens = 0;
char hundreds = 0;
char thousands = 0;

// FUNCIONES
void ONbuttonA_pressed(void);
void ONbuttonB_pressed(void);
void every5ms(void);
void __interrupt() myint(void);
void tmr0init(void);
void ADCinit(void);
void PWMinit(void);
void system_inicializacion(void);
void pinMode(char puerto, char modo);
void buzzer(int frec, long tiempo);
void serialinit(void);
void serialout(char datax);
char serialin(void);
void serialout_st(char *str);
int get_temp(void);
int get_lightlevel(void);
int analogRead(char analogpin);
void analogWrite(char pin, int dutyc);