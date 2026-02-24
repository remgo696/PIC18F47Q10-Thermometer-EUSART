/*
 * File:   maincode00.c
 * Author: klnla
 *
 * Created on 20 de febrero de 2026, 06:01 PM
 */
/* 
 * Modificaciones:
    * 25/02/2026 (remgo696): Se agrego la funcionalidad de 
    * cambiar entre grados Centigrados y Fahrenheit mediante
    * un botón.
 */

#include <xc.h>
#include "main.h"
#include "LCD.h"
#include "UART.h"
#include "DHT.h"
#define _XTAL_FREQ 32000000UL

unsigned int Temperatura, Humedad;


void configuro(void){
    //conf modulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
    OSCEN = 0x40;
    //inicializacion del LCD
    LCD_INIT();
    //inicializar el UART2
    U1_INIT(BAUD_9600);
}

void main(void) {
    configuro();
    POS_CURSOR(1,0);
    ESCRIBE_MENSAJE2("Semana 5-2 20260");
    POS_CURSOR(2,0);
    ESCRIBE_MENSAJE2("Hot Summer UPC");
    __delay_ms(3000);
    BORRAR_LCD();
    while(1){
        struct DHT_Values actuales = DHT_GetBoth(DHT22);
        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("Temp:");
        LCD_ESCRIBE_DECIMAL_VAR_INT(actuales.DHT_Temp, 3, 1);
        LCD_CHAR_GRADO();
        ESCRIBE_MENSAJE2("C");
        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("Humed:");
        LCD_ESCRIBE_DECIMAL_VAR_INT(actuales.DHT_Humid, 3, 1);
        ESCRIBE_MENSAJE2("%RH");
        U1_STRING_SEND("HOLA UPCINO");
        U1_NEWLINE();
        __delay_ms(1000);
    }
}
