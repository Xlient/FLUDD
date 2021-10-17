
#include <CloudIoTCore.h>
#include "esp8266_mqtt.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>


// Get Network time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

////////////////////
int WaterLevel,
  rainLevel ,
  Trig,
  Echo;


 bool rainDetected
 char* waterlevelStatus;
 char* waterLeakStatus;
 float waterDistance;
 

/******** Prtotypes  *******/

String toJson();
char* toJsonSensorData();
char* calculateDistance(int, int);
void postData()

//*******************************//

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  setupCloudIoT(); 
 timeClient.begin();
 timeClient.setTimeOffset(-5000);
}

static unsigned long lastMillis = 0;
void loop()
{
  if(!mqtt->loop())
  {
    mqtt->mqttConnect();
  }
  

  delay(10); 

while (Serial.available()  < 4 ) 
{
  digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
  digitalWrite(LED_BUILTIN,HIGH);

  
  }
readSensorData();

  if (millis() - lastMillis > 60000)
  {
    lastMillis = millis();
    publishTelemetry( toJsonSensorData()); // publish to the G cloud pub sub
    postData(); // post to our api as well
  } 
  
  Serial.println("sent");
  delay(60000);
  
}

String toJson()
{
 StaticJsonDocument<200> doc;

  doc["waterLevelStatus"] = waterlevelStatus 
  doc["leakStatus"] =  waterLeakStatus
  doc["waterDistance"] = waterDistance
  doc["dateReceived"] = getDateNow();
  doc["timeReceived"] = getTimeNow();

   String json;
   serializeJson(doc,json);
   return json;
}

char* toJsonSensorData()
{
  
   StaticJsonDocument<200> doc;

  doc["waterLevel"] =  WaterLevel;
  doc["rainLevel"] = rainLevel;
  doc["waterDistance"] = waterDistance
  doc["dateReceived"] = getDateNow();
  doc["timeReceived"] = getTimeNow();

   char* json;
   serializeJson(doc,json);
   return json;
}

void readSensorData(){
 
       WaterLevel = Serial.parseInt();
       rainLevel =  Serial.parseInt();
       Trig = Serial.parseInt();
       Echo = Serial.parseInt();


      if (WaterLevel < 80)
      {
        waterlevelStatus  = "All clear";
      }

      else if (WaterLevel > 80 && <= 448)
      {
        waterlevelStatus  = "Rising water level has been detected";
      }
       else 
      {
        waterlevelStatus  = "High water level has been detected";
      }

      if (rainLevel == 1){
        waterLeakStatus = "Water Leak Detected";
      }

      else
      {
         waterLeakStatus = "All clear";
      }
}

String getDateNow()
{
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime);
    int currentDay = ptm->tm_mday;
    int currentMonth = ptm->tm_mon+1;
    int currentYear = ptm->tm_year+1900;
    String currentDate =String(currentYear) + "-" + String(currentMonth) + "-" + String(currentDay);
    return currentDate;
}

char* getTimeNow()
{
    timeClient.update();
    return timeClient.getFormattedTime();
}

void setWaterDistance()
{
    float distanceInCm = (0.034 x Echo) / 2;

    waterDistance =  distanceInCm  x 30.48 // converting to Feet
}


void PostData()
{
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    http.begin(client, "http://" SERVER_IP "/postplain/"); //HTTP
    http.addHeader("Content-Type", "application/json");

    http.POST(toJson()) // posting sensor data to our API

    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      http.end();
  
  }


 
