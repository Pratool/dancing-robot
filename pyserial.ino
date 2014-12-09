char incomingByte = 0;
int WORKING_LED = 8;
int LED2 = 6;
int LED3 = 4;

void setup() {
  Serial.begin(9600);
  
  pinMode(WORKING_LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  digitalWrite(WORKING_LED, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

void loop() {
  while (Serial.available()) {
    incomingByte = Serial.read();
    //Serial.println(incomingByte);
    if (incomingByte == '0') {
      digitalWrite(WORKING_LED, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
    }
    if (incomingByte == '1') {
      digitalWrite(WORKING_LED, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
    }
    if (incomingByte == '2') {
      digitalWrite(WORKING_LED, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, HIGH);
    }
  }
}
