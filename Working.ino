#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>

#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "3.asia.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

const char* ssid = "JC";
const char* password = "frie3165";
String link="http://dweet.io/dweet/for/arduino?A0=";
String link2="&Time=";
String link1;
#define IR1 2
#define IR2 5
unsigned char occupancy = 0;
int total_occupancy = 100;
int flag1 = 0;
int flag2 = 0;

void setup () {
   timeClient.begin();
  Serial.begin(115200);
  pinMode(IR1, INPUT);
//  attachInterrupt(digitalPinToInterrupt(IR1), handleInterrupt1, RISING);
  
  pinMode(IR2, INPUT);
//  attachInterrupt(digitalPinToInterrupt(IR2), handleInterrupt2, RISING);
  pinMode(D7,OUTPUT);
  pinMode(D5,OUTPUT);
  
  Serial.begin(9600);
  
  int IR1_read,IR2_read;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
  
}
 
void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedTime());

  if(digitalRead(D2) == 0 & digitalRead(D4) == 1)
  {
    digitalWrite(D5,HIGH);
    if(flag2==0){
      flag1=1;
    }
    else{
      flag2=0;
      occupancy--;
      if(occupancy==255)
        occupancy=0;
    }
    delay(1000);
    digitalWrite(D5,LOW);
  }

  else if(digitalRead(D2) == 1  & digitalRead(D4) == 0)
  {
    digitalWrite(D7,HIGH);
      if(flag1==0){
        flag2=1;
      }
      
      else{
        flag1=0;
        occupancy++;
      }
      delay(1000);
      digitalWrite(D7,LOW);
  }
  
  /*
  //digitalWrite(D2,HIGH);
  if(digitalRead(D2)==0)
  Serial.println("HERE....");
  else
  Serial.println("THERE....");
  */
  /*
  if(occupancy < total_occupancy  & occupancy >= 0)
  {
    if(digitalRead(D2) == 0 & digitalRead(D4) == 1)
    {
      Serial.println("HERE....");
      while(digitalRead(D4));
        occupancy++;
        Serial.println(occupancy);
    }
    else if(digitalRead(D2) == 1  & digitalRead(D4) == 0 & occupancy != 0)
    {
      while(digitalRead(D2));
        occupancy--;
        Serial.println(occupancy);
    }
    delay(2000);
  }
  else if(digitalRead(D4) == 0 & occupancy != 0 )
  {
    while(digitalRead(D2));
    occupancy--;
    Serial.println(occupancy);
    delay(2000);
  }
  
  else
    Serial.println("SORRY,PARTY IS FULL");
   */
     
  link1=link+occupancy;
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin(link1);  //Specify request destination
    Serial.println(link1);
    int httpCode = http.GET();                                                                  //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
 
    http.end();   //Close connection
 
  }
  Serial.println(occupancy);
  delay(500);    //Send a request every 30 seconds
}

/*
void handleInterrupt1() {
  if(flag2==0){
    flag1=1;
  }
  else{
    flag2=0;
    occupancy--;
    if(occupancy==255)
      occupancy=0;
  }
  delay(2000);
}


void handleInterrupt2() {
  if(flag1==0){
    flag2=1;
  }
  
  else{
    flag1=0;
    occupancy++;
  }
  delay(2000);
}
*/
