
#include "arduinoSec.h"


void leerPulsadores(PULSADORES* pulsantes) {
    unsigned long tiempoActual = millis();
    if (tiempoActual - *(pulsantes->ultimo_Tiempo_Pulsadores) >= *(pulsantes->intervalo_Pulsadores)) {
        *(pulsantes->ultimo_Tiempo_Pulsadores) = tiempoActual;

        for (int i = 0; i < pulsantes->num_pul; i++) {
            bool estadoActual = digitalRead(*(pulsantes->pulsantes_list + i)) == LOW;

            if (estadoActual && !(*(pulsantes->state_list + i))) {
                // Si el pulsador se acaba de presionar
                *(pulsantes->state_list + i) = true;
                //Serial.print("Pulsador ");
                //Serial.print(i + 1);
                //Serial.println(" presionado");
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

            // Actualizar el estado de cada LED según el paso actual
            for (int i = 0; i < secuencia->numLeds; i++) {

                if (*(secuencia->sentido_sec)) // Dependiendo el sentido de la secuencia
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