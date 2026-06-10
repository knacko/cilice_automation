#include <AccelStepper.h>
#include <EasyButton.h>
#include <ezLED.h>

// Define the pins
const int dirPin   = 2;
const int stepPin  = 3;
const int forwPin  = 4;
const int revPin   = 5;
const int stopPin  = 6;
const int greenPin = 7;
const int redPin   = 8;

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

// Setup the buttons
EasyButton forwButton(forwPin);
EasyButton revButton(revPin);
EasyButton stopButton(stopPin);

// Setup the LEDs
ezLED greenLED(greenPin);
ezLED redLED(redPin);

void setup() {

  // Setup the stepper
  stepper.disableOutputs();           // Initial state is unpowered
  stepper.setMaxSpeed(maxSpeed);
  stepper.setAcceleration(maxAccel);
  bool zeroSet = false                // Becomes true once the initial position is set via the stop button

  // Initialize the buttons and set callbacks
  forwButton.begin();
  revButton.begin();
  stopButton.begin();
  forwButton.onPressed(onForward);
  revButton.onPressed(onReverse);
  stopButton.onPressed(onStop);

  // Start the LEDs
  greenLED.turnOFF();
  redLED.turnON();
}

void loop() {
  // Poll the buttons
  forwButton.read();
  revButton.read();
  stopButton.read();

  // Poll the LED
  greenLED.loop();
  redLED.loop();
}

void onForward() {
  if (zeroSet) rotateDegrees(endPos); } // Start the bend

void onReverse() {
  if (zeroSet) rotateDegrees(startPos); } // Return to the original pos

void onStop() {
  if (!zeroSet) { // Sets the zero/home position
    stepper.setCurrentPosition(0);
    stepper.enableOutputs();
    zeroSet = true;
  } else { // Stops the motor ASAP and disables the steppers
    stepper.stop();
    while (stepper.distanceToGo() != 0) {
      stepper.run(); }
    stepper.disableOutputs();
    zeroSet = false;
  }
  
  // Toggle the LEDs
  greenLED.toggle();
  redLED.toggle();
}

void rotateDegrees(float degrees) {
  stepper.enableOutputs();
  long steps = round((degrees * gearRatio) / (360/stepsPerRev));
  stepper.moveTo(steps);
  while (stepper.distanceToGo() != 0 && digitalRead(stopPin) != LOW) {
    stepper.run(); }
}