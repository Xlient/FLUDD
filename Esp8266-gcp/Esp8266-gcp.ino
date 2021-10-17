
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
  Echo;


 bool rainDetected;
 char* waterlevelStatus;
 char* waterLeakStatus;
 float waterDistance;
 

/******** Prtotypes  *******/

String toJson();
String toJsonSensorData();
void setWaterDistance();

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

while (Serial.available()  < 2 ) 
{
  digitalWrite(LED_BUILTIN,LOW);
    delay(100);
  digitalWrite(LED_BUILTIN,HIGH);

  
  }
readSensorData();

  if (millis() - lastMillis > 6000)
  {
    lastMillis = millis();
    publishTelemetry( toJsonSensorData()); // publish to the G cloud pub sub
    String json = toJson();
    PostData(json); // post to our api as well
  } 
  
  Serial.println("sent");
  delay(6000);
  
}

String toJson()
{
 StaticJsonDocument<200> doc;

  doc["waterLevelStatus"] = waterlevelStatus; 
  doc["waterLeakStatus"] =  waterLeakStatus;
  doc["waterDistance"] = waterDistance;
  doc["dateReceived"] = getDateNow();
  doc["timeReceived"] = getTimeNow();

   String json;
   serializeJson(doc,json);
   return json;
}

String toJsonSensorData()
{
  
   StaticJsonDocument<200> doc;

  doc["waterLevel"] =  WaterLevel;
  doc["rainLevel"] = rainLevel;
  doc["waterDistance"] = waterDistance;
  doc["dateReceived"] = getDateNow();
  doc["timeReceived"] = getTimeNow();

   String json;
   serializeJson(doc,json);
   return json;
}

void readSensorData(){
 
       WaterLevel = Serial.parseInt();
       rainLevel =  Serial.parseInt();
       Echo = Serial.parseInt();


      if (WaterLevel < 6)
      {
        waterlevelStatus  = "All clear";
      }

      else if (WaterLevel > 6 && WaterLevel<= 9)
      {
        waterlevelStatus  = "Rising water level has been detected";
      }
       else 
      {
        waterlevelStatus  = "High water level has been detected";
      }

      if (rainLevel == 0){
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

String getTimeNow()
{
    timeClient.update();
    return timeClient.getFormattedTime();
}

void setWaterDistance()
{
    float distanceInCm = (0.034 * Echo) / 2;

    waterDistance = distanceInCm  * 30.48; // converting to Feet
}





 
