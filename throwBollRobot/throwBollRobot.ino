#include <Servo.h>
Servo myservo; // 建立Servo物件，控制伺服馬達
int getSerialInt();
void angleWork(int readInt);
void writeAngle(int readAngle);

const int servoMotorPin = 9;
const int readThrowBollPin = 10;
const int angleMax = 180;
const int angleMin = 30;
int nowAngle = angleMax;
int minThrowTimeInterval = 4000;
long lastThrowTime = 0;
bool DEBUG = false;

void setup() {
  Serial.begin(9600);
  pinMode(readThrowBollPin,INPUT);
  myservo.attach(servoMotorPin);
  myservo.write(nowAngle);
  lastThrowTime = millis();
}

void loop() {
  
  if(DEBUG && Serial.available()){
    readChr = Serial.read();
    readInt = getSerialInt();
    angleWork(readInt);
  }
  
  // 投球機構觸發感測，因化簡為無額外功能控制
  // 故在發射部分修為直接以delay 防止其他錯誤產生
  if(digitalRead(readThrowBollPin)  
      && (millis() - lastThrowTime) > minThrowTimeInterval){
    lastThrowTime = millis();
    
    // 投球機構運作
    writeAngle(angleMin);
    delay(1000);
    writeAngle(angleMax);
    delay(1000);
  }
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

