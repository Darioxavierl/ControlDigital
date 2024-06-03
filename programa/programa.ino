/*
    Solucion a la Practica 4 de Control digital
    Codigo Realizado por:
    - David Montaño
    - Dario Portilla
*/

#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include "arduinoSec.h"
#include "arduinoSec.c"

//******************************************************************************************************************************
//**************************************************** LEDS ********************************************************************
//******************************************************************************************************************************

// Declaración de los LEDs
int leds[NUM_LEDS] = {LED1, LED2, LED3, LED4};
int estados_led[NUM_LEDS] = {LOW, LOW, LOW, LOW};

// Secuencia de ejemplo: encender y apagar LEDs en orden
int secuencia1[LONG_SEC] = {
    0b0001, // LED 1 encendido
    0b0010, // LED 2 encendido
    0b0100, // LED 3 encendido
    0b1000, // LED 4 encendido
    0b0000  // Todos apagados
};

// Variables para el retardo no bloqueante
unsigned long ultimoTiempo = 0;
unsigned long intervalo = 200; // Intervalo de tiempo en milisegundos
bool sentido_sec = true;

// Inicialización de la secuencia
Secuencia secuencia = {&leds[0],&estados_led[0], NUM_LEDS, &secuencia1[0], LONG_SEC, &ultimoTiempo, &intervalo,&sentido_sec};

//******************************************************************************************************************************


//******************************************************************************************************************************
//***************************************************** PULSANTES **************************************************************
//******************************************************************************************************************************

// Declaracion de los pulsantes
int pulsantes[NUM_PULSANTES] = {SW1,SW2,SW3,SW4};
int pulsantes_state[NUM_PULSANTES] = {LOW,LOW,LOW,LOW};
// Variables para el retardo no bloqueante en la lectura de pulsadores
unsigned long ultimoTiempoPulsadores = 0;
unsigned long intervaloPulsadores = 50; // Intervalo de tiempo en milisegundos para el debounce
unsigned long ultimoTiempoAccion = 0;
unsigned long intervaloAccion = 100; // Intervalo de tiempo en milisegundos para el debounce

// Inicializacion de los pulsantes
PULSADORES pulsantes_struct = {&pulsantes[0], &pulsantes_state[0], NUM_PULSANTES, &ultimoTiempoPulsadores,&intervaloPulsadores};
// Inicializacion para accion de los pulsantes
AccionPul accion_pulsantes = {&pulsantes_state[0],&ultimoTiempoAccion,&intervaloAccion,&sentido_sec,NUM_PULSANTES,&intervalo};

//******************************************************************************************************************************


void setup() {
    // Inicialización de los pines de los LEDs con aritmetica de punteros 
    for (int i = 0; i < secuencia.numLeds; i++) {
        pinMode(*(secuencia.pins + i), OUTPUT);
        digitalWrite(*(secuencia.pins + i), *(secuencia.estados + i));
    }

    // Inicialización de los pines de los SWs con notacion de [] 
    for (int i = 0; i < pulsantes_struct.num_pul; i++) {
        pinMode(pulsantes_struct.pulsantes_list[i], INPUT_PULLUP);  
    }
    // Iniciando Serial
    Serial.begin(115200);
}


void loop() {
    ejecutarSecuencia(&secuencia);
    leerPulsadores(&pulsantes_struct);
    funcionPulsadores(&accion_pulsantes);
}