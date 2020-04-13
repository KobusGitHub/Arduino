#include <ESP8266HTTPClient.h>

#include <IFTTTWebhook.h>

#include <IFTTTWebhook.h>
#include <ESP8266WiFi.h>

#define IFTTT_API_KEY "hKz5fItk_n_tyrrV7W5mystovzU3-tXUhwLJd1hxLe2"
#define IFTTT_EVENT_NAME "esppush"

const char* ssid="DO_F62B";
const char* password = "jonnyb55";
//const char* IFTTT_API_KEY = "hKz5fItk_n_tyrrV7W5mystovzU3-tXUhwLJd1hxLe2";
//const char* IFTTT_EVENT_NAME = "esppush"; // esppush  pushbutton

// String mainURL ="https://maker.ifttt.com/trigger/esppush/with/key/hKz5fItk_n_tyrrV7W5mystovzU3-tXUhwLJd1hxLe2";
 String mainURL ="https://www.google.com";
 
int ledPin = D1;
int buttonPin = D5;

int currentState = 0;
int newButtonState = 0;

void setup() {
  
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(9600);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  digitalWrite( ledPin , HIGH);
  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

  delay(1000);
  digitalWrite( ledPin , LOW);
  delay(1000);
 
}

void loop() {
  // put your main code here, to run repeatedly:

  
  newButtonState = digitalRead(buttonPin);
  delay(100);
 
  if(newButtonState == LOW) { // GPIO pins is normally on. So we need to ground it for an event
    if(currentState == 0){
      digitalWrite(ledPin, HIGH);
      currentState = 1;

      if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("CONNECTED");
        
        HTTPClient http;
        WiFiClient client;
        
        if (http.begin(client, mainURL)) {
          Serial.print("[HTTP] GET...\n");
  
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
      
  
           
      //      
      //      IFTTTWebhook hook(IFTTT_API_KEY, IFTTT_EVENT_NAME);
      //      hook.trigger();
      } else {
        Serial.printf("NOT CONNECTED");
      }

    } else {
      digitalWrite(ledPin, LOW);
      currentState = 0;
    }
    newButtonState = LOW;
    delay(1000);
  }

    
}
