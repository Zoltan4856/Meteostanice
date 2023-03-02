#include <M5StickCPlus.h>
#include "M5_ENV.h"
#include "WiFi.h"
#include "ThingSpeak.h"
#include <Adafruit_GFX.h>


const char* ssid = "LF";    /*Wifina*/
const char* password = "hesloksiti";   

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
  delay(1000);
  WiFi.begin(ssid,password);
  Serial.println("\nConnecting");

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  /*M5.begin();*/             // Init M5StickCPlus.  初始化M5StickCPlus
      // Rotate the screen.  旋转屏幕
    Wire.begin(32, 33);  // Wire init, adding the I2C bus.  Wire初始化, 加入i2c总线
    qmp6988.init();
}


void loop() {   
      Serial.println("\nConnected.");
  pressure = qmp6988.calcPressure();
    if (sht30.get() == 0) {  // Obtain the data of shT30.  获取sht30的数据
        tmp = sht30.cTemp;   // Store the temperature obtained from shT30.
                             
        hum = sht30.humidity;  // Store the humidity obtained from the SHT30.
                               
    } else {
        tmp = 0, hum = 0;
    }
  
   
  ThingSpeak.setField(1 , tmp);
  ThingSpeak.setField(2 , hum);
  ThingSpeak.setField(3 , pressure);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);


    
  M5.update();
  
  if (M5.BtnA.wasPressed()) {    //If the key is pressed. 
        
        M5.lcd.setRotation(3);
        M5.lcd.setTextSize(2);
        M5.lcd.setTextColor(RED);
        M5.lcd.println("Teplota: " + String(tmp));
        M5.lcd.println("Humidity: " + String(hum));
        M5.lcd.println("pressure: " + String(pressure));
  }
  if (M5.BtnB.wasPressed()) {
      M5.Lcd.qrcode("https://qr.page/g/3v4GUNy5P9S", 20, 65, 100);
    }
  else if (M5.BtnB.wasReleased()){
    M5.lcd.fillScreen(BLACK);
  }
  
    delay(1000);
  

}