
#ifndef LIB_DHT_H
/* Librer�a desarrollada por Kalun Lau en base a
un ejemplo de interface que no me acuerdo en donde lo v�

Changelog:
25/11/2024 Le coloqu� una opci�n rudimentaria de timeout
para que no se quede colgado el programa en los whiles
*/
#define	LIB_DHT_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define _XTAL_FREQ 32000000UL

/* Tipo enumerado para seleccionar el modelo de sensor DHT */
typedef enum {
    DHT11 = 0,
    DHT22 = 1
} DHT_Model_t;

/* Estructura para almacenar los valores leídos del sensor DHT */
typedef struct {
    unsigned int DHT_Temp;
    unsigned int DHT_Humid;
} DHT_Values_t;

/*Función para la etapa de inicio de comunicacion con el DHTxx*/
void DHT_Start(DHT_Model_t modelo);

/*Función para ver si el DHTxx esta disponible DHTxx*/
void DHT_Check(DHT_Model_t modelo);

/*Función para hacer lectura de un dato de 8 bits del DHTxx*/
unsigned char DHT_Read(DHT_Model_t modelo);

/*Función para obtener el valor de temperatura del DHTxx
 Tener en cuenta que para el DHT22 se deberá de dividir entre 10
 el valor obtenido*/
unsigned int DHT_GetTemp(DHT_Model_t modelo);

/*Función para obtener el valor de humedad relativa del DHTxx
 Tener en cuenta que para el DHT22 se deberá de dividir entre 10
 el valor obtenido*/
unsigned int DHT_GetHumid(DHT_Model_t modelo);

DHT_Values_t DHT_GetBoth(DHT_Model_t modelo);

#endif	/* LIB_DHT_H */