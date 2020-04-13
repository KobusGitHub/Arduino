#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

int currentState = 0;
int newState = 0;
int newButtonState = 0;
int ledPin = D1;
int buttonPin = D5;
int buttonStopStartPin = D7;
int newDBState = 0;
int newStopStartState = 0;


// TIME
const long utcOffsetInSeconds = 7200;
WiFiUDP ntpUDP;
NTPClient timeClient( ntpUDP, "pool.ntp.org", utcOffsetInSeconds); 
String currentDateTime = "";
// CONNECTION
const char* ssid = "DO_F62B";
const char* password = "jonnyb55";

// IFTTT
const char* host = "maker.ifttt.com";
const int httpsPort = 443;
const int API_TIMEOUT = 10000;  //keep it long if you want to receive headers from client
const String url = "/trigger/esppush/with/key/hKz5fItk_n_tyrrV7W5mystovzU3-tXUhwLJd1hxLe2";

// FIREBASE
#define FIREBASE_HOST "nodemcu-99611.firebaseio.com"
#define FIREBASE_AUTH "vw64nCLAm76sv8SHXjC3b0pntbD1C3T5gFVo9Hah"
#define WIFI_SSID "DO_F62B"
#define WIFI_PASSWORD "jonnyb55"
static unsigned long last_checkDb_time = 0;

// INTERRUPT
static unsigned long last_interrupt_time = 0;
int interruptFlag = 0;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonStopStartPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT); // Onboard LED -> Output

  

  // CONNECTION
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // FIREBASE
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
   
  // Timer
  //timeClient.begin();
  
  timeClient.begin();


  // INTERRUPT
  attachInterrupt(digitalPinToInterrupt(buttonPin), stateChangeFn, CHANGE);
  interruptFlag = 0;

  
  // Switch onboard led on for one second showing app is now running
  digitalWrite(LED_BUILTIN, LOW); // Onboard LED -> ON
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH); // Onboard LED -> OFF
}

// the loop function runs over and over again forever
void loop() {

  newStopStartState = digitalRead(buttonStopStartPin);
  if(newStopStartState == HIGH) // NOT CONNECTED TO GROUND
  {
    delay(1000);
  }
  else 
  {

    if(interruptFlag == 1) {
      interruptFlag = 0;
      if(currentState == 1) {
        digitalWrite(ledPin, LOW);
        currentState = 0;
        firebaseWrite(0);
        doIftttCall();
      } else {
        digitalWrite(ledPin, HIGH);
        currentState = 1;
        firebaseWrite(1);
      }
    }
    delay(100);


    unsigned long checkDb_time = millis();
    if (checkDb_time - last_checkDb_time > 2000)
    {
      last_checkDb_time = checkDb_time;
      
      newDBState = firebaseRead();
      delay(100);
     
      if(currentState != newDBState){
        if(newDBState == 1){
           digitalWrite(ledPin, HIGH);
           currentState = 1;
        } else {
           digitalWrite(ledPin, LOW);
           currentState = 0;
           doIftttCall();
        }
        delay(100);
      }
     
    }
  }
}

void setCurrentTime() {

  timeClient.update();
  currentDateTime = timeClient.getFormattedDate();

  // Extract date
  int splitT = currentDateTime.indexOf("T");
  String dayStamp = currentDateTime.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
  // Extract time
  String timeStamp = currentDateTime.substring(splitT+1, currentDateTime.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);
  

}


void doIftttCall() {
   // IFTTT
  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  client.setTimeout(API_TIMEOUT);
  
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');

  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("closing connection");
}



void firebaseWrite(int state) {
    Serial.print("SETTING STATE ON DB: " + state);
    Serial.print("SETTING DATETIME ON DB: " + currentDateTime);
    Firebase.set("STATE", state);
    Firebase.setString("DATETIME", currentDateTime);
    // handle error
    if (Firebase.failed()) {
        Serial.print("setting /number failed:");
        Serial.println(Firebase.error());  
        return;
    }
    Serial.print("SETTING STATE ON DB DONE");
    delay(100);
 }

 int firebaseRead() {
    Serial.print("STATE IN DB: ");
    int dbState = Firebase.getInt("STATE");
    Serial.println(dbState);
    delay(100);
    return dbState;
 }


 void checkTime() {

  // Time
  setCurrentTime();
  delay(100);

    
  // 2020-03-06T17:19:22Z"
  //int splitT = currentDateTime.indexOf("T");

  Serial.print("\n");

  String stringPart = "";
  
  String stringDate = "2020-03-06T17:19:22Z";
  stringPart = stringDate.substring(0, stringDate.indexOf("-")); // 2020
  Serial.print("Year: " + stringPart);
  delay(100);
  int year = stringPart.toInt();
  Serial.print("Year: " + year);
  delay(100);
  Serial.print("\n");
  delay(100);
   
  stringDate = stringDate.substring(stringDate.indexOf("-") + 1); // 03-06T17:19:22Z";
  stringPart = stringDate.substring(0, stringDate.indexOf("-")); // 03
  Serial.print("Month: " + stringPart);
  delay(100);
  int month = stringPart.toInt(); // 03
  Serial.print("Month: " + month);
  delay(100);
  Serial.print("\n");
  delay(100);
  
  stringDate = stringDate.substring(stringDate.indexOf("-") + 1); // 06T17:19:22Z";
  stringPart = stringDate.substring(0, stringDate.indexOf("T")); // 06
  Serial.print("Day: " + stringPart);
  delay(100);
  int day = stringPart.toInt(); // 06
  Serial.print("Day: " + day);
  delay(100);
  Serial.print("\n");
  delay(100);
  
  stringDate = stringDate.substring(stringDate.indexOf("T") + 1); // 17:19:22Z";
  stringPart = stringDate.substring(0, stringDate.indexOf(":")); // 17
  Serial.print("Hour: " + stringPart);
  delay(100);
  int hour = stringPart.toInt(); // 17
  Serial.print("Hour: " + hour);
  delay(100);
  Serial.print("\n");
  delay(100);
  
  stringDate = stringDate.substring(stringDate.indexOf(":") + 1); // 19:22Z";
  stringPart = stringDate.substring(0, stringDate.indexOf(":")); // 19
  Serial.print("Minute: " + stringPart);
  delay(100);
  int minute = stringPart.toInt(); // 19
  Serial.print("Minute: " + minute);
  delay(100);
  Serial.print("\n");
  delay(100);
  
  stringDate = stringDate.substring(stringDate.indexOf(":") + 1); // 22Z";
  stringPart = stringDate.substring(0, stringDate.indexOf("Z")); // 22
  Serial.print("Second: " + stringPart);
  delay(100);
  int seconds = stringPart.toInt(); // 22
  Serial.print("Second: " + seconds);
  delay(100);
  Serial.print("\n");
  delay(100);
  

  
 }




ICACHE_RAM_ATTR void stateChangeFn() { // ICACHE_RAM_ATTR makes sure function runs in RAM
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 1000)
  {
    
    Serial.println("");
    Serial.println("stateChangeFn()");

//    if(currentState == 1) {
//     firebaseWrite(0);
//    } else {
//     firebaseWrite(1);
//    }
    interruptFlag = 1;
   
  }
  last_interrupt_time = interrupt_time;
}




  
