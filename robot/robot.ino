#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>

Servo myservo; // 建立Servo物件，控制伺服馬達
SoftwareSerial BT(8,9);
byte readByte;
int readDirection;
int readSpeed;

const byte mode_motor_stop = 0;
const byte mode_Throw_motor_on = 1;
const byte mode_Throw_motor_off = 2;
const byte mode_Throw_boll_one_time = 3;

const int angleMax = 180;
const int angleMin = 30;
int nowAngle = angleMax;

const int servoMotorPin = 12;

void pinSetup();
void montorStop();
void montorWork(int directionM,int speedM);
void angleWork(int readInt);
void writeAngle(int readAngle);

// 此方向分量是以數字九宮格製作
int directionArray[10][4]  = 
{
  {0,0,0,0},
  {0,130,255},
  {0,255,0,255},
  {0,255,0,130},
  {0,255,255,0},
  {0,0,0,0},
  {255,0,0,255},
  {130,0,255,0},
  {255,0,255,0},
  {255,0,130,0}
};
const int motor1_1 = 10;
const int motor1_2 = 11;
const int motor2_1 = 5;
const int motor2_2 = 6;

const int    = 13;

long throwBollTime = -1000;

void setup() {
  pinSetup();
  BT.begin(9600);
  Serial.begin(9600);
  myservo.attach(servoMotorPin);
  myservo.write(nowAngle);
  delay(150); 
}

void loop() {
  
  if (BT.available() > 0) {
    readByte = BT.read();
    Serial.println(readByte);
    if(readByte == mode_motor_stop){
      montorStop();
    }else if(readByte == mode_Throw_motor_on || readByte == mode_Throw_motor_off){
      if(readByte == mode_Throw_motor_on){
        digitalWrite(throwMotorPin,HIGH);
        Serial.println("throwOn");
      }else{
        digitalWrite(throwMotorPin,LOW);
        Serial.println("throwOff");
      }
    }else if(readByte == mode_Throw_boll_one_time){
      writeAngle(angleMin);
      delay(1000);
      writeAngle(angleMax);
      delay(1000);
    }else{
      readDirection = readByte % 10;
      readSpeed = (int)(readByte / 10);
      Serial.print(readByte);
      Serial.print(" ");
      Serial.print(readDirection);
      Serial.print(" ");
      Serial.println(readSpeed);
      if(1 <= readDirection <= 9 ){
        if(1 <= readSpeed <= 5){
          montorWork(readDirection,readSpeed);
        }
      }
    }
  }
  
  if (Serial.available() > 0) {
    readByte = 0;
    while(Serial.available() > 0){
      char nowReadDigital = Serial.read();
      readByte = readByte*10 + nowReadDigital - '0';
    }
    if(readByte >255){
      readByte = 255;
    }else if(readByte < 0){
      readByte = 0;
    }
    
    if(readByte == mode_motor_stop){
      montorStop();
    }else if(readByte == mode_Throw_motor_on || readByte == mode_Throw_motor_off){
      if(readByte == mode_Throw_motor_on){
        digitalWrite(throwMotorPin,HIGH);
        Serial.println("throwOn");
      }else{
        digitalWrite(throwMotorPin,LOW);
        Serial.println("throwOff");
      }
    }else if(readByte == mode_Throw_boll_one_time){
      throwBollTime = millis();
    }else{
      readDirection = readByte % 10;
      readSpeed = (int)(readByte / 10);
      Serial.print(readByte);
      Serial.print(" ");
      Serial.print(readDirection);
      Serial.print(" ");
      Serial.println(readSpeed);
      if(1 <= readDirection <= 9 ){
        if(1 <= readSpeed <= 5){
          montorWork(readDirection,readSpeed);
        }
      }
    }
  }
}

void montorStop(){
  analogWrite(motor1_1,0);
  analogWrite(motor1_2,0); 
  analogWrite(motor2_1,0);
  analogWrite(motor2_2,0); 
}

void montorWork(int directionM,int speedM){
  int analogValue [4] ;
  analogValue[0] = (int)(directionArray[directionM][0] * speedM / 5);
  analogValue[1] = (int)(directionArray[directionM][1] * speedM / 5);
  analogValue[2] = (int)(directionArray[directionM][2] * speedM / 5);
  analogValue[3] = (int)(directionArray[directionM][3] * speedM / 5);
  Serial.print(analogValue[0]);
  Serial.print(analogValue[1]);
  Serial.print(analogValue[2]);
  Serial.println(analogValue[3]);
  
  analogWrite(motor1_1,analogValue[0]);
  analogWrite(motor1_2,analogValue[1]); 
  analogWrite(motor2_1,analogValue[2]);
  analogWrite(motor2_2,analogValue[3]); 
}

void pinSetup(){
  pinMode(motor1_1,OUTPUT);
  pinMode(motor1_2,OUTPUT);
  pinMode(motor2_1,OUTPUT);
  pinMode(motor2_2,OUTPUT); 
  pinMode(throwMotorPin,OUTPUT);
  myservo.attach(servoMotorPin);
  analogWrite(motor1_1,LOW);
  analogWrite(motor1_2,LOW);
  analogWrite(motor2_1,LOW);
  analogWrite(motor2_2,LOW);
  digitalWrite(throwMotorPin,LOW);
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
