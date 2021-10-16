
   
   const int waterLevelsensor = A0;
   const int  trigInput = 31;
   const int echoInput = A1;
   const int rainSesnsor = A2;
   const int rainSesnorSwitch = 30;


  int WaterLevel = 0;
  int waterDrops = 0;
  int Echo = 0;
  bool WaterDropsDetected = false;
  
  
   
   //******** Prtotypes  *******//

 void sendData();
  
   //******** Prtotypes  *******//
   
void setup() {
  
 pinMode(waterLevelsensor,INPUT);
 pinMode(trigPin,OUTPUT);
 pinMode(echoPin,INPUT);
 pinMode(rainSensor,INPUT);
 pinMode(rainSesnorSwitch,OUTPUT);



 Serial.begin(9600);
 Serial1.begin(115200);  // to talk to esp

}

void loop() {
 digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
 WaterLevel = analogRead(waterLevelsensor);
 
 waterDrops= analogRead(trigPin;
 Echo = pulseIn(echoPin, HIGH);

   
Serial.println(WaterLevel);
Serial.println(waterDrops);
Serial.println(Trig);
Serial.println(Echo);
 sendData();


 delay(60000); // wait for another hour
  
}

void sendData() 
{
    Serial1.println('start');
    Serial1.println(WaterLevel);
    Serial1.println(',');
    Serial1.println(waterDrops);
    Serial1.println(',');
    Serial1.println(Echo);

   Serial.println("report sent to esp8266");
  
  }

    
    
