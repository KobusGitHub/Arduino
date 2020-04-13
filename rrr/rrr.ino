#include <IFTTTWebhook.h>
#include <ESP8266WiFi.h>

#define ledPin 5 
#define wakePin 16
#define ssid "DO_F62B"
#define password "jonnyb55"
#define IFTTT_API_KEY "hKz5fItk_n_tyrrV7W5mystovzU3-tXUhwLJd1hxLe2"
#define IFTTT_EVENT_NAME "esppush"

void setup() {
  Serial.begin(9600);
  while(!Serial) { 
  }
  Serial.println(" ");// print an empty line before and after Button Press    
  Serial.println("Button Pressed");
  Serial.println(" ");// print an empty line  

  connectToWifi();

  //just connected to Wi-Fi
  IFTTTWebhook hook(IFTTT_API_KEY, IFTTT_EVENT_NAME);
  hook.trigger();
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);   
  delay(200);              
  digitalWrite(ledPin, LOW); 
  //now sending board to sleep

  ESP.deepSleep(wakePin); 

}

void loop() {
   //if deep sleep is working, this code will never run.
  Serial.println("This shouldn't get printed");

}


void connectToWifi() {
  Serial.print("Connecting to: SSID NAME"); //uncomment next line to show SSID name
  //Serial.print(ssid); 
  WiFi.begin(ssid, password);  
  Serial.println(" ");// print an empty line
  Serial.print("Attempting to connect: ");

  //try to connect for 10 seconds
  int i = 10;
  while(WiFi.status() != WL_CONNECTED && i >=0) {
    delay(1000);
    Serial.print(i);
    Serial.print(", ");
    i--;
  }
  Serial.println(" ");// print an empty line

  //print connection result
  if(WiFi.status() == WL_CONNECTED){
    Serial.print("Connected."); 
    Serial.println(" ");// print an empty line
    Serial.print("NodeMCU ip address: "); 
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed - check your credentials or connection");
  }
}
