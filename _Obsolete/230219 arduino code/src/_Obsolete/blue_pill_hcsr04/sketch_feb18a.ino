// STM32 built-in LED is connected with PC13 
// #define LED_BUILTIN PC13

// HC-SR04
// #define TRIG_PIN GPIO_PIN_3
//#define TRIG_PORT GPIOA
// #define ECHO_PIN GPIO_PIN_4
//#define ECHO_PORT GPIOA
#define TRIG_PIN PA3
#define ECHO_PIN PA4

long duration, cm, inches;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // start serial monitor at 9600 baud rate
  Serial.begin (9600);

  // HC-SR04 inputs & outputs
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}
 
// the loop function runs over and over again forever
void loop() {

  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage HIGH
  //digitalWrite(LED_BUILTIN, LOW);   // turn the LED on by making the voltage LOW

  // the HC-SR04 is triggered by a HIGH pulse of 10 or more microseconds.
  // give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);
 
  // Convert the time into a distance
  // cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
  
  if (inches < 4.0) {
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on by making the voltage LOW
  }
  Serial.print(inches);
  Serial.print("in");
  Serial.println();
  
  delay(500);
}

