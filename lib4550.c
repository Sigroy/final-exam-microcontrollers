#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "configbits.h"
#include "lib4550.h"
#define _XTAL_FREQ 4000000

bool buttonAflag = 0;
unsigned int buttonApressedcounter = 0;
unsigned int buttonApressedcounter2 = 0;
bool buttonBflag = 0;
unsigned int buttonBpressedcounter = 0;
unsigned int buttonBpressedcounter2 = 0;

void __interrupt() myint(void) {

    every5ms();
    ONbuttonA_pressed();
    ONbuttonB_pressed();
//    ONbuttonB_pressed();


    INTCONbits.TMR0IF = 0; // Reseteamos la bandera de interrupción
    TMR0L = 236; //Para 5 ms
    return;
}

void system_inicializacion() {
    OSCCON &= 0b10001111;
    OSCCON |= 0b01100000;

    UCONbits.USBEN = 0; //Habilitar RC4 y RC5 como entradas digitales

    UCFGbits.UTRDIS = 1;

    tmr0init();
    ADCinit();
    PWMinit();

    TRISAbits.TRISA4 = 0; //Led integrado

    PORTB_CONF = 0; //puerto B como salida
    PORTB_WRITE = 0; // 0 PARA QUE SE APAGUE TODO HE INICIALIZARLO. 
    PORTA_CONF = 0; // PUERTO A COMO SALIDA 
    PORTA_WRITE = 0; // 0 PARA QUE SE APAGUE TODO HE INICIALIZARLO.  
    PORTD_CONF = 0; //PORT D SALIDA salida
    PORTA_WRITE = 0; // 0 PARA QUE SE APAGUE TODO HE INICIALIZARLO.
}

void tmr0init() {
    INTCONbits.GIE = 1; // habilitar las interrupciones globales (configurar PEIE = 0)
    INTCONbits.PEIE = 0; // Deshablitar Interrupciones de periféricos.
    INTCONbits.TMR0IE = 1; //Habilitar interrupción por desbordamiento de TMR0
    INTCONbits.INT0IE = 0; // Deshabilitar interrupciones externas.
    INTCONbits.RBIE = 0; //Deshabilitar interrupción por puerto RB
    INTCONbits.TMR0IF = 0; // Limpiar la bandera de desbordamiento de TMR0
    RCONbits.IPEN = 0; // Deshabilitar prioridades en las interrupciones
    T0CONbits.TMR0ON = 1; // Habilitar tmr0
    T0CONbits.T08BIT = 1; //Configurar timer a 8 bits (puede ser de 16 bits)
    T0CONbits.T0CS = 0; // Seleccionar que el timer0 se incrementa por ciclo de reloj interno
    T0CONbits.PSA = 0; // Utilizar un prescaler (para hacer más largo el timpo de cuenta del timer).
    //Los siguientes 3 bits controlan el preescaler, en este caso 1:256
    T0CONbits.T0PS0 = 1;
    T0CONbits.T0PS1 = 1;
    T0CONbits.T0PS2 = 1;
    TMR0L = 236;
}

void pinMode(char puerto, char modo) {
    if (puerto == _PB0) TRISBbits.TRISB0 = modo;
    if (puerto == _PB1) TRISBbits.TRISB1 = modo;
    if (puerto == _PB2) TRISBbits.TRISB2 = modo;
    if (puerto == _PB3) TRISBbits.TRISB3 = modo;
    if (puerto == _PB4) TRISBbits.TRISB4 = modo;
    if (puerto == _PB5) TRISBbits.TRISB5 = modo;
    if (puerto == _PB6) TRISBbits.TRISB6 = modo;
    if (puerto == _PB7) TRISBbits.TRISB7 = modo;

    if (puerto == _PD0) TRISDbits.TRISD0 = modo;
    if (puerto == _PD1) TRISDbits.TRISD1 = modo;
    if (puerto == _PD2) TRISDbits.TRISD2 = modo;
    if (puerto == _PD3) TRISDbits.TRISD3 = modo;
    if (puerto == _PD4) TRISDbits.TRISD4 = modo;
    if (puerto == _PD5) TRISDbits.TRISD5 = modo;
    if (puerto == _PD6) TRISDbits.TRISD6 = modo;
    if (puerto == _PD7) TRISDbits.TRISD7 = modo;

    if (puerto == _PC0) TRISCbits.TRISC0 = modo;
    if (puerto == _PC1) TRISCbits.TRISC1 = modo;
    if (puerto == _PC2) TRISCbits.TRISC2 = modo;
    if (puerto == _PC6) TRISCbits.TRISC6 = modo;
    if (puerto == _PC7) TRISCbits.TRISC7 = modo;

    if (puerto == _PE0) TRISEbits.TRISE0 = modo;
    if (puerto == _PE1) TRISEbits.TRISE1 = modo;
    if (puerto == _PE2) TRISEbits.TRISE2 = modo;

}

void buzzer(int frec, long tiempo) {
    long tON = 500000 / frec;
    long bloque = tON / 18;
    long ciclos = tiempo * 500 / tON;

    for (int y = 0; y < ciclos; y++) {
        BUZZER_WRITE = 1;
        for (int x = 0; x < bloque; x++) {
        }
        BUZZER_WRITE = 0;
        for (int x = 0; x < bloque; x++) {
        }
    }
}


//Velocidad fija de 1200 baudios

void serialinit() {
    // Se enciende el módulo de hardware y recibe la configuración inicial
    TRISCbits.TRISC6 = 1; //RC6 como entrada digital
    TRISCbits.TRISC7 = 1; //RC7 como entrada digital

    //TXSTAbits.CSRC = 0; //Clock source select bit
    TXSTAbits.TX9 = 0; //8 bit transmission
    TXSTAbits.TXEN = 0; //Transmit enabled
    TXSTAbits.SYNC = 0; //Asynchronous mode, no tiene reloj
    TXSTAbits.SENDB = 1; //Send sync Break on next...
    //...transmission (cleared by...
    //...hardware upon completion).
    TXSTAbits.BRGH = 0; //Low speed rate


    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1; //enables receiver
    RCSTAbits.ADDEN = 0; //Disables address detection
    RCSTAbits.FERR = 0; //No overrun error


    BAUDCONbits.ABDOVF = 0;
    BAUDCONbits.RXDTP = 0;
    BAUDCONbits.TXCKP = 0;
    BAUDCONbits.BRG16 = 0;
    BAUDCONbits.ABDEN = 0;

    SPBRG = 51; //para 1200 se pone 51 a este registro
    TXREG = 0b01110111;
}

void serialout(char datax) {
    TXSTAbits.TXEN = 1; //Enciende la transmición
    while (TXSTAbits.TRMT == 0);
    TXREG = datax; //El registro manda el dato
    __delay_ms(5);
}

char serialin() {
    if (PIR1bits.RCIF) {
        return RCREG;
        PIR1bits.RCIF = 0;
    } else return ('\0');
}

void serialout_st(char *str) {
    while (*str != '\0') {
        serialout(*str);
        str++;
    }
}

void ADCinit() {
    TRISA = TRISA | 0b11101111; //Todo el puerto A como entrada
    ADCON1 = 0b00001010; // Voltajes de referencia VSS y VDD (0 y 5 volts) / RA0 y RA1 como entradas analógicas
    ADCON0 = 0b00000001; // Selección de AN0 como canal de entrada analógica
    ADCON2 = 0b10000100; //Justificación a la derecha / Reloj de conversión FOSC/4
    ADCON0bits.ADON = 1; //Convertidor AD encendido    

}

int get_temp() {
    ADCON0bits.CHS = _AN0; //seleccionar canal AN0
    unsigned int aux = 0;
    ADCON0bits.GODONE = 1; //Iniciar la conversión Analógico-Digital
    while (ADCON0bits.GODONE == 1) { //Esperar que termine la conversión.
    }
    aux = ADRESH << 8;
    aux |= ADRESL;
    aux = aux * 4.9; //convertir a grados * 10
    return aux;
}

int get_lightlevel() {
    ADCON0bits.CHS = _AN1; //seleccionar canal AN0
    unsigned int aux = 0;
    ADCON0bits.GODONE = 1; //Iniciar la conversión Analógico-Digital
    while (ADCON0bits.GODONE == 1) { //Esperar que termine la conversión.
    }
    aux = ADRESH << 8;
    aux |= ADRESL;
    return aux;
}

int analogRead(char analogpin) {
    ADCON0bits.CHS = analogpin; //seleccionar canal AN0
    unsigned int aux = 0;
    ADCON0bits.GODONE = 1; //Iniciar la conversión Analógico-Digital
    while (ADCON0bits.GODONE == 1) { //Esperar que termine la conversión.
    }
    aux = ADRESH << 8;
    aux |= ADRESL;
    return aux;
}

void PWMinit() {

    CCP1CON = 0b00001100;
    CCP2CON = 0b00001100;
    //ESTABLECER LA FRECUENCIA DE OPERACIÓN (UTILIZANDO LA FÓRMULA)
    PR2 = 124; //Para 500Hz con reloj de 4MHz
    //ESTABLECER EL PREESCALER, EN ESTE CASO 1:16
    T2CONbits.T2CKPS0 = 0;
    T2CONbits.T2CKPS1 = 1;
    //ACTIVAR EL TIMER2
    T2CONbits.TMR2ON = 1;
    //ESTABLECER EL CICLO DE TRABAJO,10 BITS EN TOTAL, EN ESTE CASO EL
    //NÚMERO 512 EN BINARIO: 10000000000
    CCPR1L = 0b00000000;
    CCP1CONbits.DC1B1 = 0;
    CCP1CONbits.DC1B0 = 0;

    CCPR2L = 0b00000000;
    CCP2CONbits.DC2B1 = 0;
    CCP2CONbits.DC2B0 = 0;
    //CONFIGURAR PUERTO C COMO SALIDA (SÓLO SE UTILIZARÁ RC2)
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;

}

void analogWrite(char pin, int dutyc) {
    int dato = 1.96 * dutyc;

    if (pin == _PC1) {
        CCPR1L = dato >> 2;
        CCP1CONbits.DC1B = dato & 0b11;
    }

    if (pin == _PC2) {
        CCPR2L = dato >> 2;
        CCP2CONbits.DC2B = dato & 0b11;
    }
}