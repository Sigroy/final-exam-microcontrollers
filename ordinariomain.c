#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pic18f4550.h>
#include "configbits.h"
#include "lib4550.h"
#define _XTAL_FREQ 4000000

void main() {
    system_inicializacion();
    serialinit();

    BUZZER_CONF = OUTPUT;
    BUZZER_WRITE = 0;


    while (1) {

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