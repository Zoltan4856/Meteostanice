#include <M5StickCPlus.h>
#include "M5_ENV.h"
#include "WiFi.h"
#include "ThingSpeak.h"
#include <Adafruit_GFX.h>
#include <LiquidCrystal.h>


const char* ssid = "vivien-mbinet.cz";    /*Wifina*/
const char* password = "vivien21122007";  


WiFiClient  client;


unsigned long myChannelNumber = 2007003;                /*pripojeni na thingspeak*/
const char * myWriteAPIKey = "G1OVAKOKYW67O8VA";       /**/


unsigned long lastTime = 0;
unsigned long timerDelay = 100;
SHT3X sht30;
QMP6988 qmp6988;


float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;


void setup() {
 M5.begin();
 Serial.begin(9600);
 WiFi.begin(ssid,password);
 Serial.println("\nConnecting");
 Serial.println("\nConnected to the WiFi network");
 Serial.print("Local ESP32 IP: ");
 Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);  // Initialize ThingSpeak
   Wire.begin(32, 33);  // Wire init, adding the I2C bus. 
   qmp6988.init();
}


int cas_odeslat = 300;


void loop() {  
 
  Serial.println("alive");
 Serial.println("Teplota: " + String(tmp));
 Serial.println("Humidity: " + String(hum));
 Serial.println("Pressure: " + String(pressure));


 pressure = qmp6988.calcPressure();
   if (sht30.get() == 0) {  // Obtain the data of shT30. 
       tmp = sht30.cTemp;   // Store the temperature obtained from shT30.
                           
       hum = sht30.humidity;  // Store the humidity obtained from the SHT30.
                             
   } else {
       tmp = 0, hum = 0;
   }
  Serial.println("Data se posilaji na Thinspeak ");




 if(cas_odeslat==0)
 { 
 ThingSpeak.setField(1 , tmp);
 ThingSpeak.setField(2 , hum);
 ThingSpeak.setField(3 , pressure);
 ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
 cas_odeslat = 300;
 }


M5.update();
M5.BtnA.read();
M5.BtnB.read();
if (M5.BtnA.isPressed()) {
    M5.lcd.setRotation(3);
    M5.lcd.fillScreen(BLACK);
    M5.lcd.setTextSize(2);
    M5.lcd.setTextColor(RED);
    M5.lcd.setCursor(0, 10);
    M5.lcd.setTextColor(GREEN);
    M5.lcd.print("Teplota: " + String(tmp));
    M5.lcd.setCursor(0, 30);
    M5.lcd.setTextColor(RED);
    M5.lcd.print("Humidity: " + String(hum));
    M5.lcd.setCursor(0, 50);
    M5.lcd.setTextColor(BLUE);
    M5.lcd.print("Pressure: " + String(pressure/100));
    
} 
else if (M5.BtnB.isPressed()) {
    
    M5.lcd.setRotation(0); // use BtnB to trigger QR code display
    M5.lcd.setCursor(0, 0);
    M5.lcd.fillRect(0, 0, 240, 240, BLACK);
    M5.Lcd.qrcode("https://qr.page/g/3v4GUNy5P9S", 20, 65, 100);
} 
else {
   M5.lcd.fillRect(0, 0, 240, 240, BLACK);
  M5.lcd.setRotation(3);

const int GRAPH_WIDTH = 175;
const int GRAPH_HEIGHT = 80;
const int GRAPH_X = 20;
const int GRAPH_Y = 25;
const int GRAPH_X_MAX = GRAPH_X + GRAPH_WIDTH - 1;
const int GRAPH_Y_MAX = GRAPH_Y + GRAPH_HEIGHT - 1;
const int GRAPH_X_LABEL = GRAPH_X + GRAPH_WIDTH / 2;
const int GRAPH_Y_LABEL = GRAPH_Y_MAX + 15; // Flip the y-axis label
const int GRAPH_NUM_POINTS = GRAPH_WIDTH;

// Initialize the graph
float values[4][GRAPH_NUM_POINTS] = {0};
int index = 0;
M5.lcd.fillRect(GRAPH_X, GRAPH_Y, GRAPH_WIDTH, GRAPH_HEIGHT, BLACK);
M5.lcd.drawLine(GRAPH_X, GRAPH_Y_MAX, GRAPH_X, GRAPH_Y, WHITE); // Flip the y-axis
M5.lcd.drawLine(GRAPH_X_MAX, GRAPH_Y_MAX, GRAPH_X, GRAPH_Y_MAX, WHITE);
M5.lcd.setCursor(GRAPH_X_LABEL, GRAPH_Y_LABEL);
M5.lcd.setTextColor(WHITE);
//M5.lcd.print("cas (s)");
M5.lcd.setCursor(GRAPH_X - 50, GRAPH_Y + 10); // Flip the x-axis label -50 puvodne
//M5.lcd.print("0");
M5.lcd.setCursor(GRAPH_X_MAX - 30, GRAPH_Y - 50); // Flip the x-axis maximum value
M5.lcd.print(GRAPH_NUM_POINTS);
// plany na budoucniost
  
//Podminky kde se meni barva podle namerenzch uidaju()
//Teplota kriticky minimun -23 ktriticky maximum 40, norma 18.33- 28.3
//Vlhkost kriticky minimun 10% kriticky maximun 60%, normal 30-50% 
//Tlak kriticky minimum 965h Pa kritickzy maximum 1060 hPa normal 960-1050hpa
M5.lcd.setCursor(GRAPH_X + 3, GRAPH_Y_MAX + 5);
M5.lcd.setTextSize(1);
M5.lcd.setTextColor(GREEN);
M5.lcd.print("Teplota (C)");

M5.lcd.setCursor(GRAPH_X + 77, GRAPH_Y_MAX + 5);
M5.lcd.setTextSize(1);
M5.lcd.setTextColor(RED);
M5.lcd.print("Vlhkost (%)");

M5.lcd.setCursor(GRAPH_X + 150, GRAPH_Y_MAX + 5);
M5.lcd.setTextSize(1);
M5.lcd.setTextColor(BLUE);
M5.lcd.print("Tlak (hPa)");

unsigned long startTime = millis();
while(true) {
  M5.update();
  M5.BtnA.read();
  M5.BtnB.read();
  // Read sensor data
  float pressure;
  pressure = qmp6988.calcPressure();
  if (sht30.get() == 0) {
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  } else {
    tmp = 0, hum = 0;
  }
  if((M5.BtnA.isPressed() == 1 || M5.BtnB.isPressed() == 1 )){
    break;
    }
  if(M5.BtnA.isReleased() == 1 && M5.BtnB.isReleased() == 1){
    ThingSpeak.setField(1 , tmp);
    ThingSpeak.setField(2 , hum);
    ThingSpeak.setField(3 , pressure);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    Serial.println("Data na thingspeak se posilaji pres while true loop");
    }
  else{
    Serial.println("Data uz se neposilaji pres while true loop ale pres void loop ");
    }
  // Update the graph
  int x = map(index, 0, GRAPH_NUM_POINTS - 1, GRAPH_X_MAX, GRAPH_X); // Flip the x-coordinates
  int y1 = map((int)tmp, -20, 60, GRAPH_Y_MAX, GRAPH_Y); // Flip the y-coordinates
  int y2 = map((int)hum, 0, 100, GRAPH_Y_MAX, GRAPH_Y); // Flip the y-coordinates
  int y3 = map((int)(pressure / 100.0), 800, 1200, GRAPH_Y_MAX, GRAPH_Y);

  // Plot the data
  values[0][index] = x;
  values[1][index] = y1;
  values[2][index] = y2;
  values[3][index] = y3;
  M5.lcd.drawPixel(values[0][index], values[1][index], RED);
  M5.lcd.drawPixel(values[0][index], values[2][index], GREEN);
  M5.lcd.drawPixel(values[0][index], values[3][index], BLUE);

  // Increment index and wrap around
  index = (index + 1) % GRAPH_NUM_POINTS;

 // Erase old data
 M5.lcd.drawPixel(values[0][index], values[1][index], BLACK);
 M5.lcd.drawPixel(values[0][index], values[2][index], BLACK);
 M5.lcd.drawPixel(values[0][index], values[3][index], BLACK);
 Serial.println("Graf");
 // Add a delay to control the graph refresh rate
 delay(1000);
 cas_odeslat--;
  }
  }
  delay(1000);
  cas_odeslat--;
}
