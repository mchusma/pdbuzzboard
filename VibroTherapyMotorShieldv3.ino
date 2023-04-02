// Include Adafruit Motor Shield library
#include <AFMotor.h>

// Initialize the four motors
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Define our target attributes
// The stimulationRate is the cycle for the sequences is set to a 667 ms cycle. In the loop() function, the stimulationRate is divided by 4 to account for four motors in the sequence. The resulting value is then combined with randomJitter() to add some variability to the time intervals between motor vibrations. The delay() function uses the sum of these two values to wait for the specified duration before moving on to the next motor in the sequence.
const int stimulationRate = 667;
// for the "noisy vCR", they introduce a 23.5% jitter in the inter-stimulus interfals, which I believe mean adjusting the stimulationRate.
const int jitterRange = 157; 

// Set up the Arduino and motors
void setup() {
  Serial.begin(9600);
  Serial.println("Motor test!");

  motor1.setSpeed(200);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(RELEASE);
  motor3.setSpeed(200);
  motor3.run(RELEASE);
  motor4.setSpeed(200);
  motor4.run(RELEASE);

  randomSeed(analogRead(0));
}

// Function to vibrate a motor for a given duration
void vibrateMotor(AF_DCMotor &motor, int duration) {
  motor.run(FORWARD);
  motor.setSpeed(255);
  delay(duration);
  motor.run(RELEASE);
}

// Function to generate a random jitter value within the jitter range
int randomJitter() {
  int jitter = random(-jitterRange, jitterRange);
  return jitter;
}

// Main loop to control motor vibrations
void loop() {
  AF_DCMotor *motors[] = {&motor1, &motor2, &motor3, &motor4};
  int motorIndices[] = {0, 1, 2, 3};

  // Shuffle motor indices
  for (int i = 3; i > 0; i--) {
    int j = random(0, i + 1);
    int temp = motorIndices[i];
    motorIndices[i] = motorIndices[j];
    motorIndices[j] = temp;
  }

  // Loop through the shuffled motor indices and vibrate each motor
  for (int i = 0; i < 4; i++) {
    int randomizedDuration = 100; // random(100, 168); // Random duration of the vibration between 100 ms and 167 ms inclusive
    vibrateMotor(*motors[motorIndices[i]], randomizedDuration);
    // Wait for a randomized time interval based on the stimulation rate and jitter
    delay(stimulationRate / 4 + randomJitter());
  }
}