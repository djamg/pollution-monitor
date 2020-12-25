#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
#define DHTTYPE DHT11   // DHT 11
uint8_t DHTPin = 4; 

String serverName = "https://ptsv2.com/t/j7okf-1608917820/post";

char ssid[] = "";     //  your network SSID (name)
char pass[] = "";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10000;

const int mq135 = 35;
const int mq5 = 34;
const int mq8 = 0;


float value1 = 0;
float value2 = 0;
float value3 = 0;
float Temperature;
float Humidity;

DHT dht(DHTPin, DHTTYPE);   

void setup(){
  Serial.begin(115200);
  dht.begin();
  pinMode(DHTPin, INPUT);

  // attempt to connect to Wifi network:

  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to WPA SSID: ");

    Serial.println(ssid);

    // Connect to WPA/WPA2 network:

    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:

    delay(5000);

  }

  // you're connected now, so print out the data:

  Serial.print("You're connected to the network");
  
  
  delay(2000);

}

void loop(){

      value1 = analogRead(mq135);
  value2 = analogRead(mq5);

  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  Serial.print("MQ135: ");
  Serial.println(value1);
  Serial.print("MQ5: ");
  Serial.println(value2);
  Serial.print("Temp: ");
  Serial.println(Temperature);
  Serial.print("Humidity: ");
  Serial.println(Humidity);

  if ((millis() - lastTime) > timerDelay) {

  
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      DynamicJsonDocument doc(2048);
      doc["MQ135"] =  String(value1);
      doc["MQ5"] =  String(value2);
      doc["Temperature"]= String(Temperature); // Gets the values of the temperature
      doc["Humidity"] = String(Humidity); // Gets the values of the humidity 
      String json;
    serializeJson(doc, json);
       
      // Your Domain name with URL path or IP address with path
      http.begin(serverName);
      
      // If you need an HTTP request with a content type: application/json, use the following:
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(json);

      // If you need an HTTP request with a content type: text/plain
      //http.addHeader("Content-Type", "text/plain");
      //int httpResponseCode = http.POST("Hello, World!");
     
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
        
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
