/*      8-Jan-21
 * Demo program for IoT Coach V2 to demonstrate the basic kit features.
 * This also supports Local OTA over WiFi link
 */
//OTA Support
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>

//MQTT Support
#include "PubSubClient.h" 

//WebSocket Support
#include <ESPAsyncWebServer.h>

#include<Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TimeLib.h>
#include"SSD1306.h"
#include <HCSR04.h>
#include <Tone32.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include "constants.h"

#define BUZZER_PIN 27
#define BUZZER_CHANNEL 0

#define ONBOARDLED 2

//For WS2812b Leds
#define WS2812_LEDS_COUNT  6
#define WS2812_LEDS_DIN  33
#define WS2812_CHANNEL   0
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(WS2812_LEDS_COUNT, WS2812_LEDS_DIN, WS2812_CHANNEL, TYPE_GRB);

//For BME280 Sensor
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;

//Open Source Hardware Icon Pad
#define TOUCHPAD 14

// Temporary variables
static char celsiusTemp[7];
static char fahrenheitTemp[7];
static char humidityTemp[7];
long elapsedTime = 0;

//NTP Time Sync to show a digital clock
#define NTP_OFFSET  19800 // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "1.asia.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

String days[7] = {"SUN","MON","TUE","WED","THR","FRI","SAT"};

/* 
 *  Replace ssid and password with your wifi credentials 
 *  else program will stuck. 
*/
const char* ssid = "";
const char* password = "";

//MQTT Server Topic
char* topic = "channels/"; 
char* mqttserver = "mqtt.thingspeak.com";

WiFiClient wifiClient;
PubSubClient client(mqttserver, 1883, wifiClient);

//Oled display object
SSD1306  display(0x3c, 21, 22);


//HCSr04 Ultrasonic Sensor
int triggerPin = 13;
int echoPin = 12;
int buzzerPin = 14;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);


String twoDigits(int digits){
  if(digits < 10) {
    String i = '0'+String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}

unsigned bmestatus;

void setup() {

  pinMode(ONBOARDLED,OUTPUT);
  
  /*
   * OLED Initialization
   * Three font types are supported ArialMT_Plain_24,ArialMT_Plain_16 and
   * ArialMT_Plain_10
   */
  display.init();
  display.setFont(ArialMT_Plain_24);
  display.flipScreenVertically();
  display.drawString(1, 20, "Setting Up.. ");
  display.display();
  display.setFont(ArialMT_Plain_16);

  
  bmestatus = bme.begin(0x76);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

    ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
      display.clear();
      display.drawString(0, 0, "Start updating");
      display.display();
      display.setFont(ArialMT_Plain_24);

    })
    .onEnd([]() {
      Serial.println("\nEnd");
      display.clear();
      display.drawString(0, 20, "Done");
      display.display();

    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      display.clear();
      display.drawString(0, 20, "Burned.." + String((progress / (total / 100))) + +"%");
      display.display();

    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      display.clear();
      //display.drawString(0, 0, "Progress" + String((progress / (total / 100)));
      //display.display();
      if (error == OTA_AUTH_ERROR) display.drawString(0, 0,"Auth Failed");
      else if (error == OTA_BEGIN_ERROR) display.drawString(0, 0,"Begin Failed");
      else if (error == OTA_CONNECT_ERROR) display.drawString(0, 0,"Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) display.drawString(0, 0,"Receive Failed");
      else if (error == OTA_END_ERROR) display.drawString(0, 0,"End Failed");
      display.display();
    });

  ArduinoOTA.begin();

  
  Serial.begin(115200);
  timeClient.begin();
  timeClient.update();


  elapsedTime = millis();

  //Initializing tone generator
  tone(BUZZER_PIN, NOTE_E4, 500, BUZZER_CHANNEL);

  //Initializing WS2812 strip
  strip.begin();
  
  Serial.print("My Ip - ");
  Serial.println(WiFi.localIP());

  //Demo Tone.
  for(int i = 0; i < 56; i++)
  {
    tone(BUZZER_PIN, MarioUW_note[i],MarioUW_duration[i]*10, BUZZER_CHANNEL);
  }
}

bool ledState = 0;
bool touchState = 0;
void loop() {
  // put your main code here, to run repeatedly: 
  ArduinoOTA.handle();
 
  if( (millis()-elapsedTime) > 1000)
    {
      UpdateData();
      elapsedTime = millis();
      digitalWrite(ONBOARDLED,ledState);
      ledState = !ledState;
    }

    if(TouchSens()< 60)
    {
      touchState = !touchState;
      delay(350);
    }
    
    if(touchState)
      rainbowOn();
    else
      rainbowOff();
              
}

/*
 * This function fetches the data from BME280 weather sensor and displays on the
 * screen perodically. It also checks the for light intensity.
 */
void UpdateData()
{
            float h = bme.readHumidity();
          // Read temperature as Celsius (the default)
          float t = bme.readTemperature();
          
          // Check if any reads failed and exit early (to try again).
          if (isnan(h) || isnan(t) ) {
            Serial.println("Failed to read from DHT sensor!");
            strcpy(celsiusTemp, "Failed");
            strcpy(fahrenheitTemp, "Failed");
            strcpy(humidityTemp, "Failed");
          }
          else{
            //float hic = dht.computeHeatIndex(t, h, false);
            dtostrf(t, 6, 1, celsiusTemp);
            //float hif = dht.computeHeatIndex(f, h);
            //dtostrf(hif, 6, 2, fahrenheitTemp);
            dtostrf(h, 6, 0, humidityTemp);  
          }
          
          int ldr = map(analogRead(A0),0,4095,0,100);
          
          display.clear();
          display.drawString(1, 0, "Humidity:"+String(humidityTemp) + " %");
          display.drawString(1, 18,"Temp     :"+String(celsiusTemp) + " c");
          display.drawString(1,36,"Light:    :"+String(ldr)+ " %");
          display.display();
          
          if(ldr < 50)
            tone(BUZZER_PIN, NOTE_C8, 500, BUZZER_CHANNEL);
          else
            noTone(BUZZER_PIN, BUZZER_CHANNEL);
          
}


int colorCode = 255;
int ledIndex = WS2812_LEDS_COUNT;
void rainbowOn()
{ 
          strip.setLedColor(ledIndex, strip.Wheel((ledIndex * 256 / WS2812_LEDS_COUNT + colorCode) & 255));
          ledIndex = ledIndex > 0 ? --ledIndex : WS2812_LEDS_COUNT;
          colorCode = colorCode > 0  ? (colorCode-2) : 255;   
}

void rainbowOff()
{
   for (int i = 0; i < WS2812_LEDS_COUNT; i++) 
    strip.setLedColor(i,0x00);
}

int TouchSens()
{
  int sample = 0;
  for(int i=0;i<32;i++)
    sample += touchRead(TOUCHPAD);
  sample = sample / 32;
  
  return sample;
}
