#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>

#define NUM_LEDS 4
#define LONG_SEC 5
#define LED1 2 // Define el pin del LED
#define LED2 3 // Define el pin del LED
#define LED3 4 // Define el pin del LED
#define LED4 5 // Define el pin del LED
#define NUM_PULSANTES 4
#define SW1 8 // Define pin del pulsante 1
#define SW2 9 // Define pin del pulsante 2
#define SW3 10 // Define pin del pulsante 3
#define SW4 11 // Define pin del pulsante 4

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

//******************************** LEDS *******************************************
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
// **********************************************************************************

//******************************** PULSANTES *******************************************
// Definicion de la structura de pulsadores
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
//**************************************************************************************

void leerPulsadores(PULSADORES* pulsantes) {
    unsigned long tiempoActual = millis();
    if (tiempoActual - *(pulsantes->ultimo_Tiempo_Pulsadores) >= *(pulsantes->intervalo_Pulsadores)) {
        *(pulsantes->ultimo_Tiempo_Pulsadores) = tiempoActual;

        for (int i = 0; i < pulsantes->num_pul; i++) {
            bool estadoActual = digitalRead(*(pulsantes->pulsantes_list + i)) == LOW;

            if (estadoActual && !(*(pulsantes->state_list + i))) {
                // Si el pulsador se acaba de presionar
                *(pulsantes->state_list + i) = true;
                Serial.print("Pulsador ");
                Serial.print(i + 1);
                Serial.println(" presionado");
            } else if (!estadoActual && (*(pulsantes->state_list + i))) {
                // Si el pulsador se acaba de soltar
                *(pulsantes->state_list + i) = false;
            }
        }
    }
}

void funcionPulsadores(AccionPul* accion_struct){
    unsigned long tiempoActual = millis();

    if (tiempoActual - *(accion_struct->ultimo_Tiempo_Accion) >= *(accion_struct->intervalo_Accion))
    {
        *(accion_struct->ultimo_Tiempo_Accion) = tiempoActual;

        for (int i = 0; i < accion_struct->num_pul; i++)
        {
            if (*(accion_struct->list_state_pul + i))
            {
                if (i == 0)
                {
                    *(accion_struct->intervalo_leds) = 200;
        
                }else if (i== 1)
                {
                    *(accion_struct->intervalo_leds) = 750;

                }else if (i==2)
                {
                    *(accion_struct->sentido) = false;

                }else if (i==3)
                {
                    *(accion_struct->sentido) = true;
                }
            }
            
        }
        
    }
    
}

void ejecutarSecuencia(Secuencia* secuencia) {
    static int pasoActual = 0;
    unsigned long tiempoActual = millis();
    
    if (tiempoActual - *(secuencia->ultimo_Tiempo) >= *(secuencia->intervalo)) {
        *(secuencia->ultimo_Tiempo) = tiempoActual;
        
        int valorPaso = secuencia->secuencia[pasoActual];
        //Serial.println(valorPaso);
        
       
            // Actualizar el estado de cada LED según el paso actual
            for (int i = 0; i < secuencia->numLeds; i++) {

                if (*(secuencia->sentido_sec))
                {
                    *(secuencia->estados + i) = (valorPaso & (1 << i)) ? HIGH : LOW;
                    digitalWrite(*(secuencia->pins + i), *(secuencia->estados + i));
                } else{
                    *(secuencia->estados + (secuencia->numLeds - 1 - i)) = (valorPaso & (1 << i)) ? HIGH : LOW;
                    digitalWrite(*(secuencia->pins + (secuencia->numLeds - 1 - i)), *(secuencia->estados + (secuencia->numLeds - 1 - i)));
                }
                
            }
        // Avanzar al siguiente paso
        pasoActual = (pasoActual + 1) % secuencia->numPasos;
    }
}

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