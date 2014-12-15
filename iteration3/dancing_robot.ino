#include <VarSpeedServo.h>

// Relevant variables for servos
VarSpeedServo servo1; //servo rotates up and down - constant rotating servo:  servo10 in PenguinDances2_ino
VarSpeedServo servo2; //servo moves side to side: servo9 in PenguinDances2_ino
VarSpeedServo servo3; //servo rotates in circle - constant rotating servo: servo11 in PenguinDances2_ino
VarSpeedServo servo4; //servo flaps wings - constant rotating servo: servo10 in PenguinDances2_ino

// Initial positions for each servo
int servo1pos = 90;
int servo2pos = 87; 
int servo3pos = 90;  
int servo4pos = 90;

// Pin information
const int servo1Pin = 9;  
const int servo2Pin = 10;
const int servo3Pin = 11;
const int servo4Pin = 12;

// Time that the robot stays awake 
const int awakeTime = 500;
double time, time2, samplingTime;

// Tells each servo when to turn left (instead of right)
boolean left_servo2 = true;
boolean left_servo3 = true;
boolean left_servo4 = true;

// Tells servo 4 when to flap (for dances 3 and 4)
int flappingCounter = 0;

// Mode variables
int mode;
int prevMode;

void setup() {
  Serial.begin(9600);        // Initializes serial monitor
  
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
  
  resetServos(); // Stops all the servos from sending signals
  
  time = millis();  // Time variables for different dances
  time2 = millis();
  samplingTime = millis();
  
  mode = 0;
  prevMode = 0;
}

void loop() {
//  mode = 4;       // For debugging purposes...
  mode = determine_mode();                // There are 5 different modes:
                                          //        0. No faces detected
  // When the mode switches               //        1. One face detected                        
  // the servos stop                      //        2. One smile detected
  if (mode != prevMode) {                 //        3. Multiple smiles detected (faces > smiles)
    resetServos();                        //        4. Multiple smiles detected (faces = smiles)
  }                                       
                                          

  // The robot continues doing the same dance for 5000 milliseconds,
  // before it switches to a different dance.
  while (millis() - samplingTime < 5000) {
    if (mode == 1) {
      firstDance();
    }
    else if (mode == 2) {
      secondDance();
    }
    else if (mode == 3) {
      thirdDance();
    }
    else if (mode == 4) {
      fourthDance();
    }
    else {
      idle();
    }
  }

  samplingTime = millis();  
  prevMode = mode;
}
                      
//// Checks what mode the robot is in depending on python input                          
int determine_mode() {
  int incomingByte = 0;
  while(Serial.available()) {            // Inputs are "0", "1", "2", "3", and "4"
    incomingByte = Serial.read() - 48;   // ASCII value of "0" = 48
  }                                     
  return incomingByte;
}
  
//// Mode 0: Arms flap around very slowly.
//// Dance move for when no one is watching (no faces detected)
void idle() {
  // Switches direction of the servo in the arms after 2500 ms
  if (millis() - time > 2500) {
    left_servo4 = !left_servo4;
    time = millis();
  }
  
  if (left_servo4) {
    servo4.write(0, 30);
  }
  else {
    servo4.write(180, 40);
  }
}

//// Mode 1: Arms flap around very quickly (servo 4) and body moves up and down (servo 1).
//// Dance move for when one person is watching (no smiling)
void firstDance() {
  // Servo 1
  servo1.write(70, 30);
  
  if (millis() - time > 1000) {
    left_servo4 = !left_servo4;
    time = millis();
  }
  
  // Servo 4
  if (left_servo4) {
    servo4.write(0, 100);
  }
  else {
    servo4.write(180, 100);
  }
}
  
//// Mode 2: Arms flap even faster. Body moves up and down faster. Also, there is occasional body tilting (servo 2).
//// Dance move for when one person is smiling
void secondDance() {
  // Servo 1
  servo1.write(50, 40);
  
  // Servo 4
  if (millis() - time > 1000) {
    left_servo4 = !left_servo4;
    time = millis();
  }
  
  if (left_servo4) {
    servo4.write(0, 150);
  }
  else {
    servo4.write(180, 150);
  }
  
  // Servo 2
  if (millis() - time > 1000) {
    left_servo2 = !left_servo2;
    time = millis();
  }
  
  if (left_servo2) {
    servo2.write(80, 50);
  }
  else {
    servo2.write(110, 50);
  }
}

//// Mode 3: Adds some rotational movement to the dancing
//// Dance move for when there are multiple faces (not all people smiling)
void thirdDance() {
  // Servo 4 - flaps 3 times at once
  if (millis() - time > 600) {
    left_servo4 = !left_servo4;
    time = millis();
    flappingCounter++;
  }
  
  if (left_servo4) {
    servo4.write(0, 100);
  }
  else {
    servo4.write(180, 100);
  }

  if (flappingCounter == 3) {
      delay(750);
      
      // Servo 3 - runs after flaps 3 times
      if (left_servo3) {
        servo3.write(3, 100, true);
      }
      else {
        servo3.write(180, 100, true);
      }
      
      left_servo3 = !left_servo3;
      flappingCounter = 0;
      time = millis();
  }
}

//// Mode 4: Everything moves!!
//// Dance move for when there are multiple faces (not all people smiling)
void fourthDance() {
  // Servo 1 - rotates only when robot leans to the left
  if (left_servo3) {
    servo1.write(60, 30);
  }
  else {
    servo1.write(87, 100);
  }
  
  // Servo 2
  if (left_servo2) {
    servo2.write(80, 50, true);
  }
  else {
    servo2.write(110, 50, true);
  }

  // Servo 4 - rotates 3 times
  if (millis() - time > 600) {
    left_servo4 = !left_servo4;
    time = millis();
    flappingCounter++;
  }
  
  if (left_servo4) {
    servo4.write(0, 100);
  }
  else {
    servo4.write(180, 100);
  }

  if (flappingCounter == 3) {
      delay(750);
      
      // Servo 3 - rotates when robot has flapped 3 times
      if (left_servo3) {
        servo3.write(60, 100, true);
      }
      else {
        servo3.write(135, 100, true);
      }
      
      left_servo2 = !left_servo2;
      left_servo3 = !left_servo3;
      flappingCounter = 0;
      time = millis();
  }
}

//// Stops all incoming pulses to each servo
void resetServos() {
  servo1.stop();
  servo2.stop();
  servo3.stop();
  servo4.stop();
  
  servo1.write(servo1pos);
  servo2.write(servo2pos);
  servo3.write(servo3pos);
  servo4.write(servo4pos);
}
