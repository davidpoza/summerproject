#include <Wire.h>
#include <SoftwareSerial.h>

int rx = 16;
int tx = 17;
SoftwareSerial sim800l(rx, tx);


void setup() {
  Serial.begin(115200);
  Serial.println("SIM800L EVB");
  sim800l.begin(115200);
  
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
