#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#define TINY_GSM_DEBUG Serial

#include <SoftwareSerial.h>
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

const char apn[]      = "orangeworld";

const char server[]   = "aventurate.com";
const char resource[] = "/favicon.ico";
const int  port       = 80;

SoftwareSerial sim800l(10,11); // RX y TX de Arduino.(pines 3 y 4 en arduino nano)
TinyGsm        modem(sim800l);
TinyGsmClient  client(modem);
HttpClient     http(client, server, port);

void setup() {
  Serial.begin(38400); // Establece la velocidad para el Monitor Serie.
  Serial.println("SIM800L EVB");
  sim800l.begin(38400); // Establece la velocidad para el sim800l.

}

void loop() {
  if (!modem.restart()) {
    Serial.println(F(" [fail]"));
    Serial.println(F("************************"));
    Serial.println(F(" Is your modem connected properly?"));
    Serial.println(F(" Is your serial speed (baud rate) correct?"));
    Serial.println(F(" Is your modem powered on?"));
    Serial.println(F(" Do you use a good, stable power source?"));
    Serial.println(F(" Try using File -> Examples -> TinyGSM -> tools -> AT_Debug to find correct configuration"));
    Serial.println(F("************************"));
    delay(10000);
  }
  Serial.print("Waiting for network...");
  if (!modem.waitForNetwork()) {
    Serial.println(" fail");
    delay(10000);
    return;
  }
  Serial.println(" success");
  
  if (modem.isNetworkConnected()) { Serial.println("Network connected"); }

  modem.simUnlock("");
  Serial.print("Modem getModemInfo: ");
  Serial.println(modem.getModemInfo());

  Serial.print(F("Connecting to "));
  Serial.print(apn);
  if (!modem.gprsConnect(apn)) {
    Serial.println(" fail");
    delay(10000);
    return;
  }
  Serial.println(" success");

  if (modem.isGprsConnected()) { Serial.println("GPRS connected"); }

  Serial.print(F("Performing HTTP GET request... "));
  http.connectionKeepAlive();
  int err = http.get(resource);
  if (err != 0) {
    Serial.println(F("failed to connect"));
    delay(10000);
    return;
  }

  int status = http.responseStatusCode();
  Serial.print(F("Response status code: "));
  Serial.println(status);
  if (!status) {
    delay(10000);
    return;
  }

  http.stop();
  Serial.println(F("Server disconnected"));
  modem.gprsDisconnect();
  Serial.println(F("GPRS disconnected"));

}                                                                               
