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

/* Flag de unidad: 0 = Celsius (default), 1 = Fahrenheit */
volatile unsigned char b_fahrenheit = 0;

/* ── ISR: Interrupción externa INT0 (botón en RB0) ─────────────────────── */
void __interrupt() INT0_ISR(void){
    PIR0bits.INT0IF = 0;        // Limpiar bandera
    b_fahrenheit ^= 1;          // Conmutar unidad °C ↔ °F
}

void configuro(void){
    // Conf módulo de oscilador
    OSCCON1 = 0x60;
    OSCFRQ = 0x06;
    OSCEN = 0x40;

    // Inicialización del LCD
    LCD_INIT();

    // Inicializar el UART2
    U1_INIT(BAUD_9600);

    // Conf botón INT0 en RB0 (INT0PPS = 0x08 por defecto, no se modifica)
    TRISBbits.TRISB0   = 1;    // RB0 como entrada
    ANSELBbits.ANSELB0  = 0;    // RB0 digital
    WPUBbits.WPUB0     = 1;    // Pull-up interno habilitado

    INTCONbits.INT0EDG  = 0;    // Flanco de bajada
    PIR0bits.INT0IF     = 0;    // Limpiar bandera
    PIE0bits.INT0IE     = 1;    // Habilitar interrupción INT0
    INTCONbits.GIE      = 1;    // Habilitar interrupciones globales
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
        DHT_Values_t actuales = DHT_GetBoth(DHT22);
        unsigned int temp_mostrar = actuales.DHT_Temp;

        POS_CURSOR(1,0);
        ESCRIBE_MENSAJE2("Temp:");

        if(b_fahrenheit){
            // Conversión: T_F*10 = (9 * T_C*10) / 5 + 320
            temp_mostrar = (unsigned int)((9u * (unsigned long)temp_mostrar) / 5u + 320u);
            LCD_ESCRIBE_DECIMAL_VAR_INT(temp_mostrar, 3, 1);
            LCD_CHAR_GRADO();
            ESCRIBE_MENSAJE2("F ");
        } else {
            LCD_ESCRIBE_DECIMAL_VAR_INT(temp_mostrar, 3, 1);
            LCD_CHAR_GRADO();
            ESCRIBE_MENSAJE2("C ");
        }

        POS_CURSOR(2,0);
        ESCRIBE_MENSAJE2("Humed:");
        LCD_ESCRIBE_DECIMAL_VAR_INT(actuales.DHT_Humid, 3, 1);
        ESCRIBE_MENSAJE2("%RH ");

        // Transmitir por 
        
        U1_STRING_SEND("T:");
        U1_VAR_INT(temp_mostrar, 3, 1);
        U1_STRING_SEND(b_fahrenheit ? "F" : "C");
        U1_STRING_SEND(" H:");
        U1_VAR_INT(actuales.DHT_Humid, 3, 1);
        U1_STRING_SEND("%");
        U1_NEWLINE();        
        U1_STRING_SEND("HOLA UPCINO");
        U1_NEWLINE();

        __delay_ms(1000);
    }
}
