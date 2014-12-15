#include <Servo.h>

// Relevant variables for servos
Servo servo1; //servo rotates up and down - constant rotating servo:  servo10 in PenguinDances2_ino
Servo servo2; //servo moves side to side: servo9 in PenguinDances2_ino
Servo servo3; //servo rotates in circle - constant rotating servo: servo11 in PenguinDances2_ino

int servo1pos = 0;  
int servo2pos = 20; 
int servo3pos = 0;  

int counter = 0;   

// Pin information
const int motionPin = 13;
const int servo1Pin = 9;  
const int servo2Pin = 10;
const int servo3Pin = 11;

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
  servo3.attach(servo3Pin);
}

void loop() {
  //boolean motion_detected = detect_motion();    // If the sensor sees something move,
  int face_detected = determine_mode();        // the robot will dance according to which mode
  time = millis();

  if(face_detected==0){
    while(millis() - time <= awakeTime){
      firstDance();
    }
  }
  else if(face_detected==1){
    while(millis() - time <= awakeTime){
      secondDance();
  }
  else{//face_detected==2  
    while(millis() - time <= awakeTime){
      thirdDance();
  }
}
                                                
int determine_mode() {  // Checks which mode the robot will dance from python input
  int incomingByte = 0;
  while (Serial.available()) {
    incomingByte = Serial.read() - 48;
    
    Serial.println(incomingByte);
  }
  return incomingByte;
}
 
// Mode 0: servo 1 moves up and down.
void firstDance() {  
  servo1.write(87);
  servo2.write(87);
}

void robot_sleep() { // Tells the robot to "sleep"
  servo1.write(93);
  servo2.write(92);
}

//Mode 1: Moves the penguin
//up and down and side to side with occasional rotations at the base
void secondDance() {
  counter = 0;
  while (counter < 20) {
    servo1.attach(10);
    //moves servo up and down (rotating)
    servo1.write(40);
    delay(5);
    //servo moves from side to side
    for (servo2pos = 20; servo2pos < 110; servo2pos++) {
      servo2.write(servo2pos);
      delay(8);
    }
    for (servo2pos = 110; servo2pos >= 20; servo2pos--) {
      servo2.write(servo2pos);
      delay (8); 
    }
    counter++;
    //stops penguin movement, rotates it from left to right a few times
  } while (counter > 20) {
    for (servo2pos = 20; servo2pos < 65; servo2pos++) {
      servo2.write(servo2pos);
      delay (10);
    }
    servo1.detach();
//    turns servo to the right
    for (int i = 0; i < 6; i++) {
      servo3.write(75);
      delay(10);
      servo3.write(110);
      delay(10);
    }
    counter = 0;
  }  
}

////Mode 3: Moves penguin from side
////to side, switches to up and down movements, then switches back
void thirdDance() {
  //moves the penguin from side to side 
  while (counter < 7) {
    for (servo2pos = 20; servo2pos < 100; servo2pos++) {
      servo2.write(servo2pos);
      delay(10);    
    }
    for (servo2pos = 100; servo2pos >= 20; servo2pos--) {
      servo2.write(servo2pos);
      delay (10); 
    }
    counter ++;
    Serial.println(counter);
    //straightens penguin up, switches to moving it up and down before switching back
    //stops the up and down servo by detaching it
  } while (counter >6) {
    for (servo2pos = 20; servo2pos < 65; servo2pos++) {
      servo2.write(servo2pos);
      delay (10);
    }
    servo1.attach(10);
    servo1.write(0);
    delay(3000);
    counter = 0;
    servo1.detach();
    for (servo2pos = 65; servo2pos >=20; servo2pos--) {
      servo2.write(servo2pos);
      delay (10); 
    }
    Serial.println(counter);
    break;
  }
  
}
