#include <Ultrasonic.h>
// L298N Motor Driver pins
#define ena 10
#define enb 11
#define in1 8
#define in2 9
#define in3 12
#define in4 13

//Mode Inputs
#define mode1 4
#define mode2 5
#define mode3 6

//Ultrasonic Sensor Objects
Ultrasonic bow(A0,A1);
Ultrasonic portside(A2,A3);
Ultrasonic starboard(A4,A5);
Ultrasonic stern(A6,A7);

//Variables
int distance1;
int distance2;
int distance3;
int distance4;

int OA;//obstacle avoidance mode
int PM; // pattern mode
int SM; // swarming mode
void forward(){
 analogWrite(ena,255);
 analogWrite(enb,255);

 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
}
void turnstarboard(){
 analogWrite(ena,255);
 analogWrite(enb,150);

 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
}
void turnportside(){
 analogWrite(ena,150);
 analogWrite(enb,255);

 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
}

void hardleftrudder(){
 analogWrite(ena,255);
 analogWrite(enb,255);

 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 digitalWrite(in3, HIGH);
 digitalWrite(in4, LOW);
}

void hardrightrudder(){
 analogWrite(ena,255);
 analogWrite(enb,255);

 digitalWrite(in1, HIGH);
 digitalWrite(in2, LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);
}

void astern(){
 analogWrite(ena,255);
 analogWrite(enb,255);

 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);
}


void obstacleMode(){
distance1 = bow.read();
distance2 = portside.read();
distance3 = starboard.read();
distance4 = stern.read();

  if (distance1<30){
    astern();
  }
  else if (distance2<20){
    turnstarboard();
  }
  else if (distance3<20){
    turnportside();
  }
  else if (distance1<30 && distance2<20){
    hardrightrudder();
  }
   else if (distance1<30 && distance3<20){
    hardleftrudder();
   }
   else {
    forward();
   }
}
void patternMode(){
distance1 = bow.read();
distance2 = portside.read();
distance3 = starboard.read();
distance4 = stern.read();

forward();
delay(10000);
turnstarboad();
delay(1000);
forward();
delay(20000);
turnportside();
delay(1000);


  if (distance3<20){
    hardleftrudder();
    delay(1000);
  }
}
void swarmMode(){
  
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(ena, OUTPUT);
pinMode(enb, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
pinMode(mode1, INPUT);
pinMode(mode2, INPUT);
pinMode(mode3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
OA = digitalRead(mode1);
PM = digitalRead(mode2);
SM = digitalRead(mode3);

if (OA == HIGH){
  obstacleMode();
}
if (PM == HIGH){
  patternMode();
}
if (SM == HIGH){
  swarmMode();
}
}
