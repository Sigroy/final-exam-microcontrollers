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

char passwordCorrecta[] = {'6', '2', '7', '0', '3'};

char modo = 1; // Modo abierto inicial
char entrada = 0;
char bandera = 1;

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

int contador = 0;
DIGITS numeros;

void password() {
    char input = 0;
    char passwordCompleta[5];
    DIGITS passwordEntrada = {0, 0, 0, 0};
    LCD_Set_Cursor(1, 4);

    input = keypadread();

    while (input > 9) {
        input = keypadread();
    }

    passwordEntrada = printNumber(input);

    buzzer(1200, 80);
    LCD_putc(42);
    passwordCompleta[0] = input;
    __delay_ms(500);

    if (passwordCompleta[0] == passwordCorrecta[0] && passwordCompleta[1] == passwordCorrecta[1] && passwordCompleta[2] == passwordCorrecta[2] && passwordCompleta[3] == passwordCorrecta[3] && passwordCompleta[4] == passwordCorrecta[4]) {
        modo = 1;
    }
}

void modoAbierto() {
    LCD_Set_Cursor(0, 0); //INICIAR CURSOR EN LÍNEA 1 (DE 2) CARACTER 1 (DE 16)
    LCD_putrs("      ABIERTO "); //ESCRIBIR UNA CADENA DE CARACTERES

    analogWrite(_PC1, 190);

    entrada = keypadread();

    while (entrada > 14) { //Se queda quí hasta que no se oprima un número de 1 a 15
        entrada = keypadread();
        __delay_ms(100);
    }

    if (entrada == 14) {
        buzzer(1200, 80);
        LCD_Clear(); //LIMPIAR LCD
        LCD_Set_Cursor(0, 0); //INICIAR CURSOR EN LÍNEA 1 (DE 2) CARACTER 1 (DE 16)
        LCD_putrs("      ARMADO "); //ESCRIBIR UNA CADENA DE CARACTERES
        modo = 0;
        __delay_ms(550);
    }
}

void modoArmado() {

    analogWrite(_PC1, 250);

    entrada = keypadread();

    if (entrada == 15) {
        buzzer(1200, 80);
        LCD_Clear(); //LIMPIAR LCD
        LCD_Set_Cursor(0, 0); //INICIAR CURSOR EN LÍNEA 1 (DE 2) CARACTER 1 (DE 16)
        LCD_putrs("  Ingrese PWD: "); //ESCRIBIR UNA CADENA DE CARACTERES

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

    int dato = 0;
    char contador = 0;
    LCD_Set_Cursor(1, 0);


    while (1) {

        if (modo == 1) {
            pinMode(_PE1, OUTPUT);
            LED_ROJO = 1;
            modoAbierto();
        }

        if (modo == 0) {
            modoArmado();
        }

        //        numeros = printNumber(entrada);
        //        LCD_putc(numeros.unidades); //mostrar en pantalla el número tecleado
        //        __delay_ms(200);
        //
        //        contador++;
        //        if (contador == 1) dato = entrada * 100;
        //        if (contador == 2) dato = dato + (entrada * 10);
        //        if (contador == 3) {
        //            dato = dato + entrada;
        //            LCD_Set_Cursor(1, 9);
        //            dato = dato * 2; //dato ahora es una variable, como ejemplo, lo estamos multiplicando por 2
        //            numeros = printNumber(dato);
        //            LCD_putc(numeros.millares); //ver el número multiplicado x 2
        //            LCD_putc(numeros.centenas);
        //            LCD_putc(numeros.decenas);
        //            LCD_putc(numeros.unidades);
        //            __delay_ms(2000);
        //            LCD_Set_Cursor(1, 0);
        //            LCD_putrs("                "); //borrar toda la segunda línea de la pantalla
        //            LCD_Set_Cursor(1, 0); //regresar el cursor a la primer posición de la segunda línea
        //            contador = 0; //resetear contador
        //        }

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

// Agregar botón B

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