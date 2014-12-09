#include <Servo.h>

// Relevant variables for servos
Servo servo1; // Up-down
Servo servo2; // Forwards-backwards

// Pin information
const int motionPin = 13;
const int servo1Pin = 9;  
const int servo2Pin = 10;
int WORKING_LED = 8;
int LED2 = 6;
int LED3 = 4;

char incomingByte = 0;

// Delay between each new sensor reading
const int sensorReadTime = 2250;

// Time that the robot stays awake 
const int awakeTime = 2250;
double time;

void setup() {
  Serial.begin(9600);        // Initializes serial monitor
  
  pinMode(motionPin, INPUT);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  
  pinMode(WORKING_LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  digitalWrite(WORKING_LED, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void robot_wakeup() {  // Wakes up the robot - servo 1 moves up and down
  servo1.write(87);
  servo2.write(87);
}

void robot_sleep() { // Tells the robot to "sleep"
  servo1.write(93);
  servo2.write(92);
}
