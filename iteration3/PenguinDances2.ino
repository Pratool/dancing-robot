#include <Servo.h>

Servo myservo9;
Servo myservo10;
Servo myservo11;

int servo9pos = 20; //servo moves side to side
int servo10pos = 0;  //servo rotates up and down - constant rotating servo
int servo11pos = 0;  //servo rotates in circle - constant rotating servo

int counter = 0;   //used in all three motions to change directions of penguin

void setup() {
  myservo9.attach(9);
  myservo10.attach(10);
  myservo11.attach(11); 
  Serial.begin(9600);
}

//motion for when the penguin detects smiles (servo.write = 2).  Moves the penguin
//up and down and side to side with occasional rotations at the base
void loop() {
  counter = 0;
  while (counter < 20) {
    myservo10.attach(10);
    //moves servo up and down (rotating)
    myservo10.write(40);
    delay(5);
    //servo moves from side to side
    for (servo9pos = 20; servo9pos < 110; servo9pos++) {
      myservo9.write(servo9pos);
      delay(8);
    }
    for (servo9pos = 110; servo9pos >= 20; servo9pos--) {
      myservo9.write(servo9pos);
      delay (8); 
    }
    counter++;
    //stops penguin movement, rotates it from left to right a few times
  } while (counter > 20) {
    for (servo9pos = 20; servo9pos < 65; servo9pos++) {
      myservo9.write(servo9pos);
      delay (10);
    }
    myservo10.detach();
//    turns servo to the right
    for (int i = 0; i < 6; i++) {
      myservo11.write(75);
      delay(10);
      myservo11.write(110);
      delay(10);
    }
    counter = 0;
  }
  
}


////motion for when penguin does not detect faces.  Moves penguin from side
////to side, switches to up and down movements, then switches back
//void loop() {
//  //moves the penguin from side to side 
//  while (counter < 7) {
//    for (servo9pos = 20; servo9pos < 100; servo9pos++) {
//      myservo9.write(servo9pos);
//      delay(10);    
//    }
//    for (servo9pos = 100; servo9pos >= 20; servo9pos--) {
//      myservo9.write(servo9pos);
//      delay (10); 
//    }
//    counter ++;
//    Serial.println(counter);
//    //straightens penguin up, switches to moving it up and down before switching back
//    //stops the up and down servo by detaching it
//  } while (counter >6) {
//    for (servo9pos = 20; servo9pos < 65; servo9pos++) {
//      myservo9.write(servo9pos);
//      delay (10);
//    }
//    myservo10.attach(10);
//    myservo10.write(0);
//    delay(3000);
//    counter = 0;
//    myservo10.detach();
//    for (servo9pos = 65; servo9pos >=20; servo9pos--) {
//      myservo9.write(servo9pos);
//      delay (10); 
//    }
//    Serial.println(counter);
//    break;
//  }
//  
//}
