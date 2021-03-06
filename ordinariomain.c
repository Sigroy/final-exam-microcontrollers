#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pic18f4550.h>
#include "configbits.h"
#include "lib4550.h"
#include "lcd.h"
#define _XTAL_FREQ 4000000

typedef struct {
    char unidades;
    char decenas;
    char centenas;
    char millares;
} DIGITS;

//                             6           2           7           0           3
char passwordCorrecta[] = {0b00000110, 0b00000010, 0b00000111, 0b00000000, 0b00000011}; // Nuestra contrase?a, 03 al final

//                   01  02  03  04  05  06  07  08  09  10
char cuenta1[] = {0, 48, 48, 48, 48, 48, 48, 48, 48, 48, 49};
char cuenta2[] = {0, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48};

char modo = 1; // Modo abierto inicial
char entrada = 0; // Entrada del keypad
int contador = 0; // Contador para los intentos de la contrase?a
char bandera = 0;
char banderaPassword = 1;

DIGITS printNumber(unsigned int dato) {

    DIGITS digitos = {48, 48, 48, 48};

    for (dato = dato; dato > 999; dato = dato - 1000) {
        digitos.millares++; //Incermentar los millares
    }
    for (dato = dato; dato > 99; dato = dato - 100) {
        digitos.centenas++; //Incermentar las centenas
    }
    for (dato = dato; dato > 9; dato = dato - 10) {
        digitos.decenas++; //Incermentar las decenas
    }
    for (dato = dato; dato > 0; dato = dato - 1) {
        digitos.unidades++; //Incrementar las unidades
    }

    return digitos;
}
DIGITS numeros;

void alarma() {
    LCD_Clear(); //LIMPIAR LCD
    LCD_Set_Cursor(0, 4); //INICIAR CURSOR EN L?NEA 1 (DE 2) CARACTER 1 (DE 16)
    LCD_putrs("ALARMA"); //ESCRIBIR UNA CADENA DE CARACTERES
    while (1) {
        buzzer(1600, 300);
        buzzer(1800, 300);
    }
}

void cuentaBloqueado() {
    LCD_Clear(); //LIMPIAR LCD
    LCD_Set_Cursor(0, 4); //INICIAR CURSOR EN L?NEA 1 (DE 2) CARACTER 1 (DE 16)
    LCD_putrs("BLOQUEADO"); //ESCRIBIR UNA CADENA DE CARACTERES
    for (char i = 10; i > 0; i--) {
        LCD_Set_Cursor(1, 6);
        LCD_putc(cuenta1[i]);
        LCD_Set_Cursor(1, 7);
        LCD_putc(cuenta2[i]);
        __delay_ms(1000);
        if (puerta == 0) {
            alarma();
        }
    }
    LCD_Clear();
    banderaPassword = 0;
}

void password() {
    while (contador < 3 && modo == 0 && banderaPassword == 1) {
        LCD_Clear(); //LIMPIAR LCD
        LCD_Set_Cursor(0, 4); //INICIAR CURSOR EN L?NEA 1 (DE 2) CARACTER 1 (DE 16)
        LCD_putrs("Ingrese PWD:"); //ESCRIBIR UNA CADENA DE CARACTERES
        char input = 0;
        char passwordCompleta[5];
        DIGITS passwordEntrada = {0, 0, 0, 0};
        LCD_Set_Cursor(1, 4);

        input = keypadread();
        while (input > 9) {
            if (puerta == 0) {
                alarma();
            }
            input = keypadread();
        }
        passwordEntrada = printNumber(input);
        buzzer(1200, 80);
        LCD_putc(42);
        passwordCompleta[0] = input;
        __delay_ms(1000);

        input = keypadread();
        while (input > 9) {
            if (puerta == 0) {
                alarma();
            }
            input = keypadread();
        }
        passwordEntrada = printNumber(input);
        buzzer(1200, 80);
        LCD_putc(42);
        passwordCompleta[1] = input;
        __delay_ms(1000);

        input = keypadread();
        while (input > 9) {
            if (puerta == 0) {
                alarma();
            }
            input = keypadread();
        }
        passwordEntrada = printNumber(input);
        buzzer(1200, 80);
        LCD_putc(42);
        passwordCompleta[2] = input;
        __delay_ms(1000);

        input = keypadread();
        while (input > 9) {
            if (puerta == 0) {
                alarma();
            }
            input = keypadread();
        }
        passwordEntrada = printNumber(input);
        buzzer(1200, 80);
        LCD_putc(42);
        passwordCompleta[3] = input;
        __delay_ms(1000);

        input = keypadread();
        while (input > 9) {
            if (puerta == 0) {
                alarma();
            }
            input = keypadread();
        }
        passwordEntrada = printNumber(input);
        buzzer(1200, 80);
        LCD_putc(42);
        passwordCompleta[4] = input;
        __delay_ms(1000);

        if (passwordCompleta[0] == passwordCorrecta[0] && passwordCompleta[1] == passwordCorrecta[1] && passwordCompleta[2] == passwordCorrecta[2] && passwordCompleta[3] == passwordCorrecta[3] && passwordCompleta[4] == passwordCorrecta[4]) {
            LCD_Clear();
            LCD_Set_Cursor(0, 4);
            LCD_putrs("CORRECTO");
            modo = 1;
            __delay_ms(2000);
            LCD_Clear();
            contador = 0;
            bandera = 1;
        } else {
            LCD_Clear();
            LCD_Set_Cursor(0, 4);
            LCD_putrs("INCORRECTO");
            __delay_ms(2000);
            LCD_Clear();
            contador++;

            if (contador == 3) {
                cuentaBloqueado();
                contador = 0;
            }
        }
    }
}

void modoAbierto() {
    LED_VERDE = 1;
    LED_ROJO = 0;
    LCD_Set_Cursor(0, 4); //INICIAR CURSOR EN L?NEA 1 (DE 2) CARACTER 1 (DE 16)
    LCD_putrs("ABIERTO"); //ESCRIBIR UNA CADENA DE CARACTERES

    analogWrite(_PC2, 127);

    entrada = keypadread();

    while (entrada > 14) { //Se queda qu? hasta que no se oprima un n?mero de 1 a 15
        entrada = keypadread();
        __delay_ms(250);
    }

    if (puerta == 0) {
        buzzer(800, 500);
    }

    if (entrada == 14) {
        if (puerta == 1) {
            buzzer(1200, 80);
            modo = 0;
        }
    }

}

void modoArmado() {
    pinMode(_PE1, OUTPUT);
    LED_VERDE = 0;
    LED_ROJO = 1;
    LCD_Clear(); //LIMPIAR LCD
    LCD_Set_Cursor(0, 4); //INICIAR CURSOR EN L?NEA 1 (DE 2) CARACTER 1 (DE 16)
    LCD_putrs("ARMADO"); //ESCRIBIR UNA CADENA DE CARACTERES
    __delay_ms(100);

    analogWrite(_PC2, 255);

    entrada = keypadread();

    while (entrada > 14) { //Se queda qu? hasta que no se oprima un n?mero de 1 a 15
        if (puerta == 0) {
            alarma();
        }
        entrada = keypadread();
        __delay_ms(100);
    }

    if (entrada == 14) {
        buzzer(1200, 80);
        banderaPassword = 1;
        password();
    }
}

void main() {
    system_inicializacion();
    serialinit();

    BUZZER_CONF = OUTPUT;
    BUZZER_WRITE = 0;

    ADCinit();

    keypad4x4init();
    LCD lcd = {&PORTD, 5, 4, 0, 1, 2, 3}; // PORT, RS, EN, D4, D5, D6, D7
    LCD_Init(lcd); //Inicializar LCD
    LCD_Clear(); //LIMPIAR LCD


    while (1) {

        if (modo == 1) {
            modoAbierto();
        }

        if (modo == 0) {
            modoArmado();
        }
    }
}

void ONbuttonA_pressed() {

    if (buttonA && buttonAflag) {
        buttonApressedcounter++;
        buttonApressedcounter2++;
    }


    if (buttonA && !buttonAflag) { //as soon as it's pressed
        buttonAflag = 1;
        buttonApressedcounter = 0;
        buttonApressedcounter2 = 0;

    }

    if (!buttonA && buttonAflag && buttonApressedcounter2 < 399) { //relase button before 2 sec
        buttonAflag = 0;

        //your code here


    }

    if (buttonA && buttonApressedcounter == 399) { //keep pressing during 2 sec
        buttonApressedcounter = 0;
        buttonApressedcounter2 = 400;
        //your code here

    }
    if (!buttonA) buttonAflag = 0;

}

// Agregar bot?n B

void ONbuttonB_pressed() {

    if (buttonB && buttonBflag) {
        buttonBpressedcounter++;
        buttonBpressedcounter2++;
    }


    if (buttonB && !buttonBflag) { //as soon as it's pressed
        buttonBflag = 1;
        buttonBpressedcounter = 0;
        buttonBpressedcounter2 = 0;

    }

    if (!buttonB && buttonBflag && buttonBpressedcounter2 < 399) { //relase button before 2 sec
        buttonBflag = 0;

        //your code here


    }

    if (buttonB && buttonBpressedcounter == 399) { //keep pressing during 2 sec
        buttonBpressedcounter = 0;
        buttonBpressedcounter2 = 400;
        //your code here


    }
    if (!buttonB) buttonBflag = 0;

}

void every5ms() {

}