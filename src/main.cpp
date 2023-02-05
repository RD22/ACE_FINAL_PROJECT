#include <Arduino.h>
#include "Motor.h"


//sonar LEFT
const int trig_LEFT = 17;
const int echo_LEFT = 16;
//sonar FRONT
const int trig_FRONT = 19;
const int echo_FRONT = 18;
//sonar RIGHT
const int trig_RIGHT = 21;
const int echo_RIGHT = 20;

// defines variables
long duration_LEFT;
int dist_LEFT;

long duration_FRONT;
int dist_FRONT;

long duration_RIGHT;
int dist_RIGHT;

const int speed = 250;

typedef struct {
  int state, new_state;

  // tes - time entering state
  // tis - time in state
  unsigned long tes, tis;
} fsm_t;

fsm_t robot;

// Set new state
void set_state(fsm_t& fsm, int new_state)
{
  if (fsm.state != new_state) {  // if the state changed tis is reset
    fsm.state = new_state;
    fsm.tes = millis();
    fsm.tis = 0;
    ////Serial.println(fsm_left.state);
  }
}

unsigned long interval, last_cycle;
unsigned long loop_micros;

void setup()
{
  
  Serial.begin(9600); // Starts the serial communication
  Serial.println("Boot");
  rbt_init(); 
  delay(1000);
  
  //sonar
  pinMode(trig_LEFT, OUTPUT); // Sets the trig as an Output
  pinMode(echo_LEFT, INPUT); // Sets the echo as an Input

  pinMode(trig_RIGHT, OUTPUT); // Sets the trig as an Output
  pinMode(echo_RIGHT, INPUT); // Sets the echo as an Input

  pinMode(trig_RIGHT, OUTPUT); // Sets the trig as an Output
  pinMode(echo_RIGHT, INPUT); // Sets the echo as an Input
  
  set_state(robot, 0);
  interval = 40;

}
void loop()                     
{
   unsigned long now = millis();
  if (now - last_cycle > interval) {
    loop_micros = micros();
    last_cycle = now;
  
  //LEFT SONAR
  // Clears the trigPin
  digitalWrite(trig_LEFT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on 255 state for 10 micro seconds
  digitalWrite(trig_LEFT, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_LEFT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_LEFT = pulseIn(echo_LEFT, HIGH);
  // Calculating the distance
  dist_LEFT = duration_LEFT * 0.0343 / 2;

  //FRONT SONAR
  // Clears the trigPin
  digitalWrite(trig_FRONT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on 255 state for 10 micro seconds
  digitalWrite(trig_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_FRONT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_FRONT = pulseIn(echo_FRONT, HIGH);
  // Calculating the distance
  dist_FRONT = duration_FRONT * 0.0343 / 2;
  
  //RIGHT SONAR
  // Clears the trigPin
  digitalWrite(trig_RIGHT, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on 255 state for 10 micro seconds
  digitalWrite(trig_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_RIGHT, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration_RIGHT = pulseIn(echo_RIGHT, HIGH);
  // Calculating the distance
  dist_RIGHT = duration_RIGHT * 0.0343 / 2;

  Serial.print("Distance LEFT: ");
  Serial.print(dist_LEFT); Serial.print("\t");
  Serial.print("Distance RIGHT: ");
  Serial.print(dist_RIGHT); Serial.print("\t");
  Serial.print("Distance FRONT: ");
  Serial.print(dist_FRONT); Serial.print("\n");

  if(robot.state == 0 && robot.tis >1000 && !(dist_LEFT<25) && !(dist_FRONT<25) && !(dist_RIGHT<25)){
      robot.new_state = 1;
    } else if(robot.state == 1 && (dist_LEFT<25 || dist_FRONT<25 || dist_RIGHT<25)){
      robot.new_state = 2;
    }  if(robot.state == 2 && !(dist_LEFT<25) && !(dist_FRONT<25) && !(dist_RIGHT<25)){
      robot.new_state = 11;
    } else if(robot.state == 11 && (dist_LEFT<25 || dist_FRONT<25 || dist_RIGHT<25)){
      robot.new_state = 2;
    } else if(robot.state == 2 && dist_FRONT<25){
      robot.new_state = 13;
    } else if(robot.state == 2 && dist_LEFT<25 && dist_FRONT>50 && dist_RIGHT>50){
      robot.new_state = 3;
    } else if(robot.state == 13 && dist_LEFT<25 && dist_FRONT>50){
      robot.new_state = 3;
    } else if(robot.state == 2 && dist_RIGHT<25){
      robot.new_state = 12; 
    } else if(robot.state == 12 && dist_LEFT<25 && dist_FRONT>50){
      robot.new_state = 3;
    } else if(robot.state == 3 && dist_LEFT<15){
      robot.new_state = 21;
    } else if(robot.state == 3 && dist_LEFT>25){
      robot.new_state = 22;
    } else if(robot.state == 21 && dist_LEFT>20){
      robot.new_state = 3;
    } else if(robot.state == 21 && dist_FRONT<25){
      robot.new_state = 4;
    } else if(robot.state == 22 && dist_FRONT<25){
      robot.new_state = 4;
    } else if(robot.state == 22 && dist_LEFT<20){
      robot.new_state = 3;
    } else if(robot.state == 3 && dist_FRONT<25){
      robot.new_state = 4;
    } else if(robot.state == 4 && dist_FRONT>50 && dist_LEFT<25){
      robot.new_state = 3;
    } 
    
    // Update the states
    set_state(robot, robot.new_state);

    // Actions set by the current state of the first state machine
    if(robot.state == 0){
      rbt_stop();
    } else if(robot.state == 1 ){
      rbt_move(FWD,speed);
    } else if(robot.state == 2){
      rbt_stop();
    } else if(robot.state == 3){
      rbt_move(FWD,speed);
    } else if(robot.state == 4){
      rbt_move(RIGHT,speed);
    } else if(robot.state == 11){
      rbt_move(FWD,speed);
    } else if(robot.state == 12){
      rbt_move(RIGHT,speed);
    } else if(robot.state == 13){
      rbt_move(RIGHT,speed);
    } else if(robot.state == 21){
      rbt_move(SOFT_RIGHT,speed);
    } else if(robot.state == 22){
      rbt_move(SOFT_LEFT,speed);
    }
 
  Serial.println(robot.state);
  }
}