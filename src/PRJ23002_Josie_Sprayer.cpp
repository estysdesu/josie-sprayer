//#include "avr8-stub.h"
//#include "app_api.h"
#include <Arduino.h>
#include <RunningMedian.h>
#include <Stepper.h>

// SENSOR
const float TARGET_RANGE = 61.0; // sensor target range [cm] -- 61cm ~= 24in
const int N_SAMPLES = 50; // number of samples to take
const float N_SAMPLES_AVG = 40; // number samples of median samples to average
RunningMedian voltageSamples = RunningMedian(N_SAMPLES);
RunningMedian distSamples = RunningMedian(N_SAMPLES);

// STEPPER
const int STEPS=200; // motor steps (360/<step angle>)
Stepper stepper(STEPS, 4, 5, 6, 7);


float DAC(unsigned int analogueValue) {
  return map(analogueValue, 0, 1023, 0, 5000) / 1000.0;
}

float sensorConvertDist(float voltageValue) {
  return 65 * pow(voltageValue,  -1.10);
}

void setup() {
  // DEBUGGING
  //debug_init(); // init GDB stub 
  Serial.begin(9600);   // start serial monitor at 9600 baud rate

  // VISUAL FEEDBACK -- BUILTIN LED INITIALIZE LOW
  pinMode(LED_BUILTIN, OUTPUT);   // initialize digital pin LED_BUILTIN as an output.
  digitalWrite(LED_BUILTIN, LOW); // initialize LED off

  // STEPPER -- SET DEFAULT SPEED
  stepper.setSpeed(480); // RPM
  // STEPPER -- SET STANDBY MODE INITIALLY
  pinMode(PIN3, INPUT);
  digitalWrite(PIN3, HIGH);

}
 
void loop() {
    
    for (int i=0; i<N_SAMPLES; i++) {
      
      float sensorVoltage = DAC(analogRead(A0));

      voltageSamples.add(sensorVoltage); // push a value to the bucket
      distSamples.add( sensorConvertDist(sensorVoltage) ); // push a value to the bucket
      
      delay(10);

    }

  float voltage = voltageSamples.getAverage(N_SAMPLES_AVG);
  float dist = distSamples.getAverage(N_SAMPLES_AVG);

  if (voltage < 0.5 || voltage > 2.8) {
    dist = NAN; // sensor is not stable outside of this voltage range
    digitalWrite(LED_BUILTIN, LOW); // ensure LED off
    digitalWrite(PIN3, HIGH); // ensure motor driver in standby
  } else if (dist < TARGET_RANGE) {
    digitalWrite(LED_BUILTIN, HIGH); // LED on
    digitalWrite(PIN3, LOW); // take the motor driver out of standby
    stepper.step(-STEPS*5); // rotate motor 5 times
  } else {
    digitalWrite(LED_BUILTIN, LOW); // ensure LED off
    digitalWrite(PIN3, HIGH); // ensure motor driver in standby
  }

  Serial.print("v_out:");
  Serial.print(voltage);
  Serial.print("V");
  Serial.println();
  Serial.print("dist:");
  Serial.print(dist);
  Serial.print("cm");
  Serial.println();

}

