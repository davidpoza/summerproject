#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>

int rx = 16;
int tx = 17;
SoftwareSerial sim800l(rx, tx);
TinyGsm modem(sim800l);


void setup() {
  Serial.begin(115200);
  Serial.println("SIM800L EVB");
  delay(10);

  sim800l.begin(115200);
  delay(10000);

  int initRes = modem.restart();

  if (initRes) {
    Serial.println("init OK");
  } else {
    Serial.println("init FAIL");
  }
  
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem Info: ");
  Serial.println(modemInfo);
  
  modem.waitForNetwork(600000L);
  Serial.println("registered!");
  
  int res = modem.sendSMS("+34630082850", "Esto es una prueba");
  if (res) {
    Serial.println("OK");
  } else {
    Serial.println("FAIL");
  }

}

void loop() {
  /**if(sim800l.available()) // Si bytes disponibles para leer.
  {
    Serial.println("sim800l disponible");
    Serial.println(sim800l.readString()); // Lee desde el sim800l e imprime por el Monitor Serial.
  }

  if(Serial.available()) // Si bytes disponibles para leer.
  {
    Serial.println("esp32 disponible");
    sim800l.println(Serial.readString()); // Envia los comandos AT que lee desde el Monitor Serial al sim800l.
  }
  **/
}
