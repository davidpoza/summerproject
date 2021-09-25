#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//On ESP32: GPIO-21(SDA), GPIO-22(SCL)
#define OLED_RESET -1 //Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //See datasheet for Address
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int rx = 16;
int tx = 17;
SoftwareSerial ss(rx, tx);


TinyGPS gps;

void setup() {
  Serial.begin(115200);
  //Begin serial communication Arduino IDE (Serial Monitor)

  //Begin serial communication Neo6mGPS
  ss.begin(9600);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
  delay(2000);

}

void loop() {
    
  boolean newData = false;
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
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
    print_speed(flat, flon);
  }
  else
  {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.setTextSize(3);
    display.print("No Data");
    display.display();
  }  
  
}

void print_speed(float lat, float lon)
{
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

 //String gps_speed = String(gps.speed.kmph());
  display.setTextSize(1);
  
  display.setCursor(25, 5);
  display.print("Lat: ");
  display.setCursor(50, 5);
  display.print(lat,6);

  display.setCursor(25, 20);
  display.print("Lng: ");
  display.setCursor(50, 20);
  display.print(lon,6);



  display.display();
}
