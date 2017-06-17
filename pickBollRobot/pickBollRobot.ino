#include <SoftwareSerial.h>
#include <Wire.h>
SoftwareSerial BT(8,9); //藍芽腳位，TX為PIN8，RX為PIN9

void pinSetup();
void montorStop();
void montorWork(int directionM,int speedM);
void threadReadMode(byte readByte);
int getSerialInt();

const byte mode_motor_stop = 0;
const byte mode_Throw_motor_on = 1;
const byte mode_Throw_motor_off = 2;
const byte mode_Throw_boll_one_time = 3;
const int throwMotorPin = 12;
const int throwBollPin = 13;
long lastThrowTime = 0;
// 預設多少時間給投球機器人感測投球
long throwTimeChangeTime = 1000;
bool DEBUG = false;

/* 此方向陣列是以類計算機上的數字九宮格
中的五為中心，像八個方向發散
對應值是左右馬達的最高轉速。*/
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



void loop(){
  if (BT.available() > 0) {
    byte readByte = BT.read();
    if(DEBUG)Serial.println(readByte);
    // 分析藍芽到的接收資訊
    threadReadMode(readByte);
  }
  
  if (DEBUG && Serial.available() > 0) {
    // 因Serial讀取為字元讀取，輸入十進位資料需轉換
    byte readByte = getSerialInt();
    threadReadMode(readByte);
  }
  
  // 給予投球機器人的狀態感測
  if((millis() - lastThrowTime) > throwTimeChangeTime){
    //原為傳送藍芽資料，而不需要此行
    // 預設感應間隔時間到，關閉觸發投球腳位
    digitalWrite(throwBollPin,LOW);
  }
}

void pinSetup(){
  pinMode(motor1_1,OUTPUT);
  pinMode(motor1_2,OUTPUT);
  pinMode(motor2_1,OUTPUT);
  pinMode(motor2_2,OUTPUT); 
  pinMode(throwMotorPin,OUTPUT);
  pinMode(throwBollPin,OUTPUT);
  analogWrite(motor1_1,LOW);
  analogWrite(motor1_2,LOW);
  analogWrite(motor2_1,LOW);
  analogWrite(motor2_2,LOW);
  digitalWrite(throwMotorPin,LOW);
  digitalWrite(throwBollPin,LOW);
}

void threadReadMode(byte readByte){
  if(readByte == mode_motor_stop){
      montorStop();
  }else if(readByte == mode_Throw_motor_on || readByte == mode_Throw_motor_off){
    if(readByte == mode_Throw_motor_on){
      //原為藍芽傳送資料，後改為單一機器人而做修改
      digitalWrite(throwMotorPin,HIGH);
      if(DEBUG)Serial.println("throwOn");
    }else{
      //原為藍芽傳送資料，後改為單一機器人而做修改
      digitalWrite(throwMotorPin,LOW);
      if(DEBUG)Serial.println("throwOff");
    }
  }else if(readByte == mode_Throw_boll_one_time){
    //原為藍芽傳送資料，後改為單一機器人而做修改
    digitalWrite(throwBollPin,HIGH);
    lastThrowTime = millis();
  }else{
    // 十位數 為方向的資訊
    int readDirection = readByte % 10;
    // 個位數 為速度大小的資訊
    int readSpeed = (int)(readByte / 10);
    if(DEBUG){
      Serial.print(readByte);
      Serial.print(" ");
      Serial.print(readDirection);
      Serial.print(" ");
      Serial.println(readSpeed);
    }
    // 核對資料標準
    if(1 <= readDirection <= 9 ){
      if(1 <= readSpeed <= 5){
        montorWork(readDirection,readSpeed);
      }else if(DEBUG){
        Serial.print("readSpeedEroor:");
        Serial.println(readSpeed);
      }
    }else if(DEBUG){
      Serial.print("readDirectionEroor:");
      Serial.println(readDirection);
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
  // 速度調整，將類比輸出直接以五段速作為控制依據
  for(int i=0;i<4;i++){
    analogValue[i] = (int)(directionArray[directionM][i] * speedM / 5);
    if(DEBUG)Serial.print(analogValue[i]);
  }
  analogWrite(motor1_1,analogValue[0]);
  analogWrite(motor1_2,analogValue[1]); 
  analogWrite(motor2_1,analogValue[2]);
  analogWrite(motor2_2,analogValue[3]); 
}

int getSerialInt(){
  char readChr;
  int readInt = 0;
  while(Serial.available()){
    readChr = Serial.read();
    readInt = readInt*10 + readChr - '0';
    delay(10);
  }
  // 資料閥值設定
  if(readInt >255){
    readInt = 255;
  }else if(readInt < 0){
    readInt = 0;
  }
  return readInt;
}
