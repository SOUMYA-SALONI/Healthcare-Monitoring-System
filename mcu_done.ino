#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266HTTPClient.h>

String URL="http://api.thingspeak.com/update?api_key=67QU9K1DFTA2RVDN&field1=";
// https://api.thingspeak.com/update?api_key=67QU9K1DFTA2RVDN&field1=37&field2=97

void setup() {
  Serial.begin(9600);
  WiFi.disconnect();
  delay(2000);
  Serial.print("Start connection");
  WiFi.begin("realme 5 Pro","subhasis123");
  while((!(WiFi.status()== WL_CONNECTED))){
      delay(200);
      Serial.print("..");
    }
  Serial.println("Connected");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    String data=Serial.readStringUntil('\n');
    Serial.println(data);
    int comma=data.indexOf(',');
    int has = data.indexOf('#');
    if(comma!=-1){
      float temp=data.substring(0,comma).toFloat();
//      float po=data.substring(comma+1).toFloat();
      float po=data.substring(comma+1,has).toFloat();
      float ecg = data.substring(has+1).toFloat();
      Serial.print(temp);
      Serial.print("\t");
      Serial.print(po);
      Serial.print("\t");
      Serial.println(ecg);
      sendData(temp,po,ecg);
    }
  }

}
void sendData(float temp, float po, float ecg){
   WiFiClient client;
   HTTPClient http;
   String newUrl=URL+String(temp)+"&field2="+String(po)+"&field3="+String(ecg);
   http.begin(client,newUrl);
   int responsecode=http.GET();
   String data=http.getString();
   Serial.println(data);
   http.end();
  
}
