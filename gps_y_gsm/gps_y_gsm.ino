#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_USE_GPRS true
#define TINY_GSM_USE_WIFI false
#define TINY_GSM_DEBUG Serial
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C //See datasheet for Address

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <SoftwareSerial.h>
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <TinyGPS.h>

const char apn[]      = "orangeworld";
const char server[]   = "vsh.pp.ua";
const char resource[] = "/TinyGSM/logo.txt";
const int  port       = 80;
const int GATE_PIN = 3;

SSD1306AsciiWire oled;
SoftwareSerial sim800l(10,11);
SoftwareSerial neo6m(8, 9);
TinyGsm        modem(sim800l);
TinyGsmClient client(modem);
HttpClient     http(client, server, port);
TinyGPS gps;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void reset() {
  oled.println(F("FAIL! reseting"));
  digitalWrite(GATE_PIN, LOW);
  delay(1000);
  digitalWrite(GATE_PIN, HIGH);
  delay(1000);
  resetFunc(); 
}

void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  oled.begin(&Adafruit128x64, SCREEN_ADDRESS);
  oled.setFont(System5x7);
  oled.clear();
  pinMode(GATE_PIN, OUTPUT);
  digitalWrite(GATE_PIN, HIGH);
  delay(7000);
  Serial.begin(38400); // Establece la velocidad para el Monitor Serie.
  oled.println(F("Booting up..."));
  sim800l.begin(38400); // Establece la velocidad para el sim800l.
}

void loop() {
  if (!modem.restart()) {
    oled.println(F("Failed modem restart..."));
    reset();
  }
  oled.print(F("Waiting for network..."));
  if (!modem.waitForNetwork( )) {
    reset();
    return;
  }
  oled.println(F("Success!. connected to network"));
  
  if (!modem.isNetworkConnected()) {
    oled.println(F("Not connected..."));
    reset();
  }

  oled.println(F("Network connected")); 
  modem.simUnlock("");
  oled.clear();
  
  oled.print(F("Connecting "));
  oled.println(apn);
  if (!modem.gprsConnect(apn)) {
    oled.println(F("Failed APN connection"));
    reset();
    return;
  }
  if (modem.isGprsConnected()) { oled.println(F("GPRS connected")); }

  oled.clear();
  oled.println(F("HTTP GET request..."));
 // http.connectionKeepAlive();
  int err = http.get(resource);
  while (err != 0) {
    oled.println(F("ERR con to server"));
    oled.println(err);
    delay(1000);
    err = http.get(resource);
  }

  int status = http.responseStatusCode();
  oled.print(F("Response code: "));
  oled.println(status);

  // GPS ----
  boolean newData = false;
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (neo6m.available())
    {
      char c = neo6m.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  //If newData is true
  if(newData == true)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    printCoords(flat, flon);
  }
  else
  {
    oled.clear();
    oled.println(F("No Data"));
  }  

  // -----

  http.stop();
  oled.println(F("Server disconnected"));
  modem.gprsDisconnect();
  oled.println(F("GPRS disconnected"));

}              

                                                                 

void printCoords(float lat, float lon)
{
  oled.clear();
  oled.print("Lat: ");
  oled.println(lat);
  oled.print("Lng: ");
  oled.println(lon);

}
