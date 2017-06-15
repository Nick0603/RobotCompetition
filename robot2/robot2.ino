#include <Servo.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#define motor1 5
#define motor2 6
#define servoMotor 9

Servo myservo; // 建立Servo物件，控制伺服馬達
char readChr;
int readInt;

bool autoRota = false;

const int autoTurnOnValue =520;
const int angleMax = 170;
const int angleMin = 50;

int nowAngle = angleMax;
long lastRotaTime = 0;

SoftwareSerial BT(10,11); //定義Arduino PIN10及PIN11分別為TX及RX腳位

void setup() 
{ 
  Serial.begin(9600);
  BT.begin(9600);
  pinMode(motor1,OUTPUT);
  pinMode(motor2,OUTPUT);
  myservo.attach(servoMotor); // 連接數位腳位9，伺服馬達的訊號線
  myservo.write(nowAngle);
  analogWrite(motor1,0);
  analogWrite(motor2,0);
  lastRotaTime = millis();
} 

void loop() 
{
  if(Serial.available()){
    readChr = Serial.read();
    readInt = getSerialInt();
    if( readChr == 'a'){
      if(readInt == autoTurnOnValue){
        autoRota = autoRota ^ true;
      }else if(!autoRota){
        angleWork(readInt);
      }
    }else if(readChr == 'r'){
        rpmWork(readInt);
    }
  }
  
  if(BT.available()){
    readChr = BT.read();
    readInt = getBTInt();
    if( readChr == 'a'){
      if(readInt == autoTurnOnValue){
        autoRota = autoRota ^ true;
      }else if(!autoRota){
        angleWork(readInt);
      }
    }else if(readChr == 'r'){
        rpmWork(readInt);
    }
  }
  
  if(autoRota && millis() - lastRotaTime > 3000 ){
    lastRotaTime = millis();
    writeAngle(angleMin);
    writeAngle(angleMax);
  }
  delay(10);
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

int getBTInt(){
  char readChr;
  int readInt = 0;
  while(BT.available()){
    readChr = BT.read();
    readInt = readInt*10 + readChr - '0';
    delay(10);
  }
  return readInt;
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

void rpmWork(int readInt){
    if(readInt > 255){
        readInt = 255;
    }else if(readInt < 0){
        readInt = 0;
    }
    Serial.print("motor analogWrite: ");
    Serial.println(readInt);
    analogWrite(motor1, readInt);
    analogWrite(motor2, readInt);
}

void writeAngle(int readAngle){
  while(nowAngle != readAngle){
    if(readAngle > nowAngle){
      nowAngle += 5;
      if(nowAngle > readAngle){
        nowAngle = readAngle;
      }
    }else{
      nowAngle -= 5;
      if(nowAngle < readAngle){
        nowAngle = readAngle;
      }
    }
    myservo.write(nowAngle);
    delay(30);
  }
}

