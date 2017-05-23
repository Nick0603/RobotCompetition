#include <SoftwareSerial.h>
#include <Wire.h>

SoftwareSerial BT(8, 9);
byte readByte;
int readDirection;
int readSpeed;

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

void setup() {
  pinSetup();
  BT.begin(9600);
  Serial.begin(9600);
  delay(150); 
}

void loop() {
  
  if (BT.available() > 0) {
    readByte = BT.read();
    Serial.println(readByte);
    if(readByte == 0 ){
      montorStop();
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
    readByte = Serial.read();
    
    if(readByte == '0'){
      montorStop();
    }else{
      readDirection = (readByte - '0');
      readByte = Serial.read();
      readSpeed = (readByte - '0');
      Serial.print(readDirection);
      Serial.print(" ");
      Serial.println(readSpeed);
      if(1 <= readDirection <= 9 ){
        if(1 <= readSpeed <= 5){
          montorWork(readDirection,readSpeed);
        }
      }
    }
    delay(20);
    while(Serial.available() > 0){
      readByte = Serial.read();
      delay(20);
    }
  }
  
  delay(10);
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
  analogWrite(motor1_1,0);
  analogWrite(motor1_2,0);
  analogWrite(motor2_1,0);
  analogWrite(motor2_2,0);
}

