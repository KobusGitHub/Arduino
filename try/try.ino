#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;
String mainURL ="http://ddas.cc/httpTest/?";
String user="user=ahmad22";
float temperature=45.3;
char type='j';


int ledPin = D1;
int buttonPin = D5;

int currentState = 0;
int newButtonState = 0;



void setup() {

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);


  Serial.begin(9600);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("DO_F62B", "jonnyb55");

}

void loop() {
  
  newButtonState = digitalRead(buttonPin);
  delay(100);
 
  if(newButtonState == LOW) { // GPIO pins is normally on. So we need to ground it for an event
    if(currentState == 0){
      digitalWrite(ledPin, HIGH);
      currentState = 1;



  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin...");
    if (http.begin(client, mainURL+user+"&temp="+temperature+"&type="+type)) {  // HTTP


      Serial.print("[HTTP] GET...");
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect");
    }
  }

  delay(10000);

    } else {
      digitalWrite(ledPin, LOW);
      currentState = 0;
    }
    newButtonState = LOW;
    delay(1000);
  }

    
}
