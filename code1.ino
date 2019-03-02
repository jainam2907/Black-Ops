<br data-mce-bogus="1">
 
#include &lt;Timer.h&gt;
Timer t;
 
#include &lt;Wire.h&gt;
#include &lt;LiquidCrystal.h&gt;
#include &lt;SoftwareSerial.h&gt;
 
const int sensor=A1; // Assigning analog pin A1 to variable 'sensor'
float tempc; //variable to store temperature in degree Celsius
float tempf; //variable to store temperature in Fahreinheit
float vout; //temporary variable to hold sensor reading
 
void setup()
{
pinMode(sensor,INPUT); // Configuring pin A1 as input
 
Serial.begin(9600);
Serial.begin(115200); // Baud rate of esp8266
 
///////////////////////////////////////////////
//
// Connecting WIFI
//
//
 
Serial.begin(115200); // Baud rate of esp8266
Serial.println(F("Connecting Wifi...."));
Serial.println(F("Connecting Wifi...."));
 
connect_wifi("AT",1000); // time in milliseconds
connect_wifi("AT+CWMODE=1",6000); // setting esp to station mode
connect_wifi("AT+CWQAP",6000); // setting esp to quik access point
connect_wifi("AT+RST",6000); // reseting esp
connect_wifi("AT+CWJAP=\"Ener Your Wifi Name Here\",\"Enter Your Wifi Password\"",6000); // Connecting wifi to router with SSID and Password.
//connect_wifi("AT+CWJAP=\"Testing\",\"12345\"",6000); // For example Wifi name= Testing &amp; Password=12345
 
delay(5000);
Serial.println(F("Wifi Connected"));
 
delay(2000);
 
t.every(3000, send2server); // 1000 milli seconds = 1 minute // timer.h class object
 
}
 
void loop()
{
vout=analogRead(sensor);
vout=(vout*500)/1023;
tempc=vout; // Storing value in Degree Celsius
Serial.print("Temperature in Degree Celsius=");
 
Serial.print(tempc);
delay(1000);
 
t.update();
}
 
void send2server()
{
 
vout=analogRead(sensor);
vout=(vout*500)/1023;
tempc=vout; // Storing value in Degree Celsius
 
String postUrl="/esp8266/wifidataphp?temp=";
postUrl.concat(tempc);
 
Serial.println(postUrl);
httpGet("192.168.43.73", postUrl, 80);
 
}
 
void httpGet(String ip, String path, int port)
{
 
String atHttpGetCmd = "GET "+path+" HTTP/1.0\r\n\r\n"; // GET /status.php?slot1=1&amp;slot2=0&amp;slot3=0...
Serial.println(atHttpGetCmd);
//AT+CIPSTART="TCP","192.168.20.200",80
String atTcpPortConnectCmd = "AT+CIPSTART=\"TCP\",\""+ip+"\","+port+""; /// establishing a TCP conection with ip and port
Serial.println(atTcpPortConnectCmd);
connect_wifi(atTcpPortConnectCmd,1000);
int len = atHttpGetCmd.length();
Serial.println(len);
String atSendCmd = "AT+CIPSEND=";
atSendCmd+=len;
connect_wifi(atSendCmd,1000); /// sending data length to server for confirmation of its length.
connect_wifi(atHttpGetCmd,1000); // sending data to server
Serial.println(ip);
}
 
void connect_wifi(String cmd, int t)
{
int temp=0,i=0;
while(1)
{
//lcd.clear();
//lcd.print(cmd);
Serial.println(cmd);
Serial.println(cmd); // sending commands to esp8266 stored in cmd variable.
while(Serial.available())
{
if(Serial.find("OK"))
 
i=8;
}
delay(t);
if(i&gt;5)
break;
i++;
}
if(i==8)
{
Serial.println(F("OK"));
//lcd.setCursor(0,1);
//lcd.print("OK");
}
else
{
Serial.println(F("Error"));
// lcd.setCursor(0,1);
// lcd.print("Error");
}
}
 
<pre>
