#include <SoftwareSerial.h>

// Enviar comandos AT desde el Monitor Serie al SIM800L (Arduino UNO) 

SoftwareSerial sim800l(10, 11); // RX y TX de Arduino.

void setup() {
  Serial.begin(38400); // Establece la velocidad para el Monitor Serie.
  Serial.println("SIM800L EVB");
  sim800l.begin(38400); // Establece la velocidad para el sim800l.
}

void loop() {
  if(sim800l.available()) // Si bytes disponibles para leer.
  {
    Serial.println(sim800l.readString()); // Lee desde el sim800l e imprime por el Monitor Serial.
  }

  if(Serial.available()) // Si bytes disponibles para leer.
  {
    sim800l.println(Serial.readString()); // Envia los comandos AT que lee desde el Monitor Serial al sim800l.
  }
}
