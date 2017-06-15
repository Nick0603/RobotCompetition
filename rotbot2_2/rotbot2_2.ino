#include <Servo.h>

Servo myservo; // 建立Servo物件，控制伺服馬達

int getSerialInt();
void angleWork(int readInt);
void writeAngle(int readAngle);

const int angleMax = 180;
const int angleMin = 30;
int nowAngle = angleMax;

const int servoMotorPin = 9;
const int readThrowBollPin = 10;


bool readFlag = false;

long lastThrowTime = 0;
int throwCount = 0;


char readChr;
int readInt;

void setup() {
  Serial.begin(9600);
  myservo.attach(servoMotorPin);
  myservo.write(nowAngle);
  pinMode(readThrowBollPin,INPUT);
  lastThrowTime = millis();
}

void loop() {
  if(Serial.available()){
    readChr = Serial.read();
    readInt = getSerialInt();
    if( readChr == 'a'){
      angleWork(readInt);
    }
  }
  
  if(digitalRead(readThrowBollPin)  && (millis() - lastThrowTime) > 6000){
    lastThrowTime = millis();
    writeAngle(angleMin);
    delay(1000);
    writeAngle(angleMax);
    delay(1000);
  }
  
  delay(10);
}

void writeAngle(int readAngle){
  while(nowAngle != readAngle){
    if(readAngle > nowAngle){
      nowAngle += 10;
      if(nowAngle > readAngle){
        nowAngle = readAngle;
      }
    }else{
      nowAngle -= 10;
      if(nowAngle < readAngle){
        nowAngle = readAngle;
      }
    }
    myservo.write(nowAngle);
    delay(50);
  }
}


void angleWork(int readInt){
    if(readInt > 180){
      readInt = 180;
    }else if(readInt < 0){
      readInt = 0;
    }
    Serial.print("angle: ");
    Serial.println(readInt);
    writeAngle(readInt);
}

int getSerialInt(){
  char readChr;
  int readInt = 0;
  while(Serial.available()){
    readChr = Serial.read();
    readInt = readInt*10 + readChr - '0';
    delay(10);
  }
  return readInt;
}

