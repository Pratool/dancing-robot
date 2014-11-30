#include <Servo.h>

// Relevant variables for servos
Servo servo1; // Up-down
Servo servo2; // Forwards-backwards

// Pin information
const int motionPin = 13;
const int servo1Pin = 9;  
const int servo2Pin = 10;

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
}

void loop() {
  boolean motion_detected = detect_motion();    // If the sensor sees something move,
  boolean face_detected = detect_face();        // the robot will wake up (stays awake if face detected)
  time = millis();                              
                                                
  if(face_detected || motion_detected) {        // Otherwise, the robot will sleep
    while(millis() - time <= awakeTime) {  
      robot_wakeup();
    }
  }
  else {
    robot_sleep();
    delay(sensorReadTime);
  }
}

boolean detect_motion() {  // Reads from the PIR motion sensor
  int motion = digitalRead(motionPin);
  
  if(motion == LOW) {
    return true;
  }
  return false;
}

boolean detect_face() {  // Checks if there's a face
  int incomingByte = 0;
  while (Serial.available()) {
    incomingByte = Serial.read() - 48;
    
    Serial.println(incomingByte);
  }
  
  if (incomingByte == 1) {
    return true;
  }
  return false;
}

void robot_wakeup() {  // Wakes up the robot - servo 1 moves up and down
  servo1.write(87);
  servo2.write(87);
}

void robot_sleep() { // Tells the robot to "sleep"
  servo1.write(93);
  servo2.write(92);
}
