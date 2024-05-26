#ifndef arduinoSec_H
#define arduinoSec_H

#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//*********************************************************************
//************* Definicion de pines del Arduino************************
//*********************************************************************

#define LED1 2 // Define el pin del LED
#define LED2 3 // Define el pin del LED
#define LED3 4 // Define el pin del LED
#define LED4 5 // Define el pin del LED
#define SW1 8 // Define pin del pulsante 1
#define SW2 9 // Define pin del pulsante 2
#define SW3 10 // Define pin del pulsante 3
#define SW4 11 // Define pin del pulsante 4

//*********************************************************************

//*********************************************************************
//********* Definicion de cantidad de elementos************************
//*********************************************************************

#define NUM_LEDS 4
#define LONG_SEC 5
#define NUM_PULSANTES 4

//*********************************************************************

//*********************************************************************
//****************** Definicion de cestructuras************************
//*********************************************************************

// Definición de la estructura Secuencia
typedef struct {
    int* pins;
    int* estados;
    int numLeds;
    int* secuencia;
    int numPasos;
    unsigned long* ultimo_Tiempo;
    unsigned long* intervalo;
    bool* sentido_sec;
} Secuencia;

// Definicion de las structuras para los pulsadores
typedef struct {
    int* pulsantes_list;
    int* state_list;
    int num_pul;
    unsigned long* ultimo_Tiempo_Pulsadores;
    unsigned long* intervalo_Pulsadores;
} PULSADORES;

typedef struct{
    int* list_state_pul;
    unsigned long* ultimo_Tiempo_Accion;
    unsigned long* intervalo_Accion;
    bool* sentido;
    int num_pul;
    unsigned long* intervalo_leds;
} AccionPul;

//*********************************************************************

//*********************************************************************
//****************** Definicion de funciones **************************
//*********************************************************************
void leerPulsadores(PULSADORES* pulsantes);
void funcionPulsadores(AccionPul* accion_struct);
void ejecutarSecuencia(Secuencia* secuencia);
//*********************************************************************




#endif //arduinoSEC_H