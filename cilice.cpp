#include <AccelStepper.h>

// Define the pins
const int dirPin  = 2;
const int stepPin = 3;
const int forwPin = 4;
const int revPin  = 5;
const int stopPin = 6;

// Set the steps
const float stepsPerRev = 200.0;
const float gearRatio   = 4;                  // Ratio of the gearbox (e.g, 4 if 4:1 gearbox is used)
const float maxRPM      = 100;                // RPM with maximum torque for motor
const float maxSpeed    = maxRPM*stepsPerRev;
const float maxAccel    = maxSpeed/2;

// Set the positions
const float startPos = 0    // This should correspond to the outside pin being center-right of the middle pin
const float endPos   = -270   // This should correspond to the outside pin being center-bottom of the middle pin

// Define motor interface
AccelStepper stepper(1, stepPin, dirPin);

void setup() {
  pinMode(forwPin, INPUT_PULLUP);     // Triggers LOW when pressed
  pinMode(revPin,  INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);
  stepper.disableOutputs();           // Initial state is unpowered
  stepper.setMaxSpeed(maxSpeed);
  stepper.setAcceleration(maxAccel);
  bool zeroSet = false                // Becomes true once the initial position is set via the stop button
}

void loop() {
  if (digitalRead(revPin) == LOW) {
    if (zeroSet) rotateDegrees(startPos); }

  if (digitalRead(forwPin) == LOW) {
    if (zeroSet) rotateDegrees(endPos); }

  if (digitalRead(stopPin) == LOW) {
    stepper.stop();
    while (stepper.distanceToGo() != 0) {
      stepper.run(); }
    delay(100);
    stepper.disableOutputs(); 
    stepper.setCurrentPosition(0);
    zeroSet = true;
  }
}

void rotateDegrees(float degrees) {
  stepper.enableOutputs();
  long steps = round((degrees * gearRatio) / (360/stepsPerRev));
  stepper.moveTo(steps);
  while (stepper.distanceToGo() != 0 && digitalRead(stopPin) != LOW) {
    stepper.run(); }
}