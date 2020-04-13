/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

char data;
String SerialData="";
int buttonState = 0;

int ledPin = D1;
int buttonPin = D2;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(D2, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  while(Serial.available())
  {
    delay(15);
    data=Serial.read();
   
    SerialData+=data;

    Serial.println(SerialData);
    if(SerialData=="off")
    {
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }
  
    if(SerialData=="on")
    {
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    }
    
  }

  delay(1000);
  
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
    
  
  SerialData="";

}
  
  
