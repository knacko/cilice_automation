#include <AccelStepper.h>

// Define the pins
const int dirPin = 2;
const int stepPin = 3;
const int forwardPin = 4;
const int reversePin = 5;
const int stopPin = 6;

// Set the steps
const float stepsPerRev = 200.0;
const float degPerStep = 360.0;

// Set the positions
const float startPos = 0
const float endPos = -270*degPerStep

// Define motor interface type (1 for External Driver like A4988/DRV8825)
AccelStepper stepper(1, stepPin, dirPin);

void setup() {
  pinMode(forwardPin, INPUT_PULLUP); // Button to GND, triggers LOW when pressed
  pinMode(reversePin, INPUT_PULLUP);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
  bool zeroSet = false
}

void loop() {
  // If the zero button is pressed
  if (digitalRead(reversePin) == LOW) {
  
	if (!zeroSet) {
		stepper.setCurrentPosition(0);
	} else {
		stepper.runToNewPosition(0); 
	}
	
    // Step 1: Move the motor backward until the switch is hit
    // (Adjust speed/direction depending on your switch placement)
    while (digitalRead(buttonPin) == LOW) {
      stepper.setSpeed(-200); // Move CCW slowly
      stepper.runSpeed();
    }
    
    // Step 2: Once switch is released, motor stops
    stepper.stop();
    
    // Step 3: Crucial step - define this exact location as zero
    stepper.setCurrentPosition(0);
  }

  // Continuously run stepper routines when needed
  stepper.run();
}

void rotateDegrees(float degrees) {
  long steps = round(degrees / degPerStep);
  stepper.moveTo(steps);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
}