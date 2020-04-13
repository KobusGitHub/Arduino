#include <ESP8266WiFi.h>

int blinkPin = D1;
int ledPin = D5;
const uint8_t interruptPin = D2;

int ledOn = 0;
int blinkLedOn = 0;

// CONNECTION
const char* ssid = "DO_F62B";
const char* password = "jonnyb55";

// ICACHE_RAM_ATTR void btnHighFn();
// ICACHE_RAM_ATTR void btnLowFn();

static unsigned long last_interrupt_time = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(blinkPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  // pinMode(btnPin, INPUT_PULLUP);
  //pinMode(buttonStopStartPin, INPUT_PULLUP);
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

  // Interupt
  attachInterrupt(digitalPinToInterrupt(interruptPin), btnHighFn, RISING);


  ledOn = 0;
  blinkLedOn = 0;
  
  // Switch onboard led on for one second showing app is now running
  digitalWrite(LED_BUILTIN, LOW); // Onboard LED -> ON
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH); // Onboard LED -> OFF
  
}



ICACHE_RAM_ATTR void btnHighFn() {
  // static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {


    
    Serial.println("");
    Serial.println("btnHighFn()");
    ledOn = 1;
  
    // digitalWrite(ledPin, HIGH);
    attachInterrupt(digitalPinToInterrupt(interruptPin), btnLowFn, FALLING);
  

  }
  last_interrupt_time = interrupt_time;
}

ICACHE_RAM_ATTR void btnLowFn() {

  // static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
   
    Serial.println("");
    Serial.println("btnLowFn()");
    
    ledOn = 0;
    //digitalWrite(ledPin, LOW);

    attachInterrupt(digitalPinToInterrupt(interruptPin), btnHighFn, RISING);
  }
  last_interrupt_time = interrupt_time;
}



void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);

  if(blinkLedOn == 1) {
    digitalWrite(blinkPin, LOW);
    blinkLedOn = 0;
  } else {
    digitalWrite(blinkPin, HIGH); 
    blinkLedOn = 1;
  }
 

  Serial.println("");
  Serial.println(ledOn);
  if(ledOn == 1){
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}
