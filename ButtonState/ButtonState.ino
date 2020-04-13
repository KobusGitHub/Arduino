
int currentState = 0;
int newButtonState = 0;
int ledPin = D1;
int buttonPin = D5;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT); // Onboard LED -> Output

  // Switch onboard led on for one second showing app is now running
  digitalWrite(LED_BUILTIN, LOW); // Onboard LED -> ON
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH); // Onboard LED -> OFF
}

// the loop function runs over and over again forever
void loop() {
  
  newButtonState = digitalRead(buttonPin);
  delay(100);
  Serial.println(newButtonState);
  
  if(newButtonState == LOW) { // GPIO pins is normally on. So we need to ground it for an event
    if(currentState == 0){
      digitalWrite(ledPin, HIGH);
      currentState = 1;
    } else {
      digitalWrite(ledPin, LOW);
      currentState = 0;
    }
    newButtonState = LOW;
    delay(1000);
  }
    
  
 

}
  
