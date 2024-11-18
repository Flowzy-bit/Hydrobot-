#define IRR 3
#define IRL 2
#define a 4
#define b 5
#define c 6
#define in1 8
#define in2 7
#define ena 9
#define enb 10
#define in3 11
#define in4 12

#include <Ultrasonic.h>

Ultrasonic ultrasonic1(A0,A1);
Ultrasonic ultrasonic2(A2,A3);//comment out 

int distance1;
int distance2;
int irr;
int irl;
int mode1;
int mode2;
int mode3;

void forward(){
  analogWrite(ena, 255);
  analogWrite(enb, 255);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

}

void reverse(){
  analogWrite(ena, 200);
  analogWrite(enb, 200);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void left(){
  analogWrite(ena, 200);
  analogWrite(enb, 200);

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void right(){

  analogWrite(ena, 200);
  analogWrite(enb, 200);

  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void pathfinder(){

  distance1 = ultrasonic1.read();
  distance2 = ultrasonic2.read();//comment out

  if (distance1 < 30){
    reverse();
    delay(3000);
    left();
    delay(1000);
    }
    else if (distance1 > 30 && irl == LOW){
      forward();
      delay(1000);
      right();
      delay(1000);
    }
    else if (distance1 > 30 && irr == LOW){
      forward();
      delay(1000);
      left();
      delay(1000);
    }
    else if (distance1 < 30 && irr == LOW && irl == LOW){
      reverse();
      delay(2000);
      right();
      delay(1000);
    }
    else if (distance2 < 15 ){
      forward();
      delay(2500);
    }//comment out
  else {
    forward();
  }
}

void objavoid(){
    Serial.println(distance1);
irr =  digitalRead(IRR);
irl = digitalRead(IRL);

if (irr == LOW){
  left();
  delay(400);
  Serial.println("OBJ at RIGHT, Turning LEFT");
}
else if (irl == LOW){
  right();
  delay(400);
  Serial.println("OBJ at LEFT, turning RIGHT");
}
else if (irr == LOW && irl == LOW){
  reverse();
  delay(400);
  Serial.println(" OBJ at BOTH SIDE, BACK tracking");
}

else if (irr == HIGH && irl == HIGH){
  pathfinder();
  Serial.println("NO OBJ detected, Moving");
}
}

void swarm(){
  left();
  delay(4000);
  right();
  delay(4000);
}
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(IRR, INPUT);
pinMode(IRL, INPUT);
pinMode(a, INPUT);
pinMode(b, INPUT);
pinMode(c, INPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(ena, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
pinMode(enb, OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
mode1 = digitalRead(a);
mode2= digitalRead(b);
mode3= digitalRead(c);

if (mode1 == HIGH){
  objavoid();
}
else if (mode2 == HIGH){
  swarm();
}
else if (mode3 == HIGH){
  analogWrite(ena, 0);
  analogWrite(enb, 0);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
else{
  analogWrite(ena, 0);
  analogWrite(enb, 0);
}
delay(50);
}