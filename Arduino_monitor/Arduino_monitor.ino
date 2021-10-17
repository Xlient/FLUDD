
   
   const int waterLevelsensor = A0;
   const int waterLevelsensorSwitch = 4;
   const int  trigPin = 31;
   const int echoPin = A1;
   const int rainSensor = 2;
   const int rainSensorSwitch =3 ;


  int WaterLevel = 0;
  int waterDrops = 0;
  int Echo = 0;
  bool WaterDropsDetected = false;
  
  
   
   //******** Prtotypes  *******//

 void sendData();
  
   //******** Prtotypes  *******//
   
void setup() {
  
 pinMode(waterLevelsensor,INPUT);
 pinMode(waterLevelsensorSwitch,OUTPUT);

 digitalWrite(waterLevelsensorSwitch,LOW);
 pinMode(trigPin,OUTPUT);
 pinMode(echoPin,INPUT);
 
 pinMode(rainSensorSwitch,OUTPUT);
 digitalWrite(rainSensorSwitch,LOW);



 Serial.begin(9600);
 Serial1.begin(115200);  // to talk to esp

}

void loop() {
  Serial.println("Hello");
 digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  digitalWrite(waterLevelsensorSwitch, HIGH);
  delay(10);
 WaterLevel = analogRead(waterLevelsensor);

 digitalWrite(rainSensorSwitch, HIGH); 
  delay(10);  
 waterDrops= digitalRead(rainSensor);
 digitalWrite(rainSensorSwitch, LOW); 
 Echo = pulseIn(echoPin, HIGH);

   
Serial.println(WaterLevel);
Serial.println(waterDrops);
Serial.println(Echo);
 sendData();


 delay(6000); // wait for another Min
  
}

void sendData() 
{
    Serial1.println("start");
    Serial1.println(WaterLevel);
    Serial1.println(',');
    Serial1.println(waterDrops);
    Serial1.println(',');
    Serial1.println(Echo);

   Serial.println("report sent to esp8266");
  
  }

    
    
