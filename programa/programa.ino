#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>

#define LED_PIN 13 // Define el pin del LED

void setup() {
  // Configura el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);
  // Iniciar la comunicación serial
  Serial.begin(9600); // Establecer la velocidad en baudios

  // Imprimir un mensaje de inicio
  Serial.println("Iniciando programa...");
}

void loop() {
  // Enciende el LED
  digitalWrite(LED_PIN, 1); // Utiliza 1 para representar un estado alto (HIGH)
  Serial.println("Led encendido...");
  delay(1000); // Espera 1 segundo (en milisegundos)
  

  // Apaga el LED
  digitalWrite(LED_PIN, 0); // Utiliza 0 para representar un estado bajo (LOW)
  Serial.println("Led apagado...");
  delay(1000); // Espera 1 segundo (en milisegundos)
}
