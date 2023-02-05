#include "Motor.h"

void rbt_init()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
 
}

void rbt_stop(){
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
}

void rbt_move(direction_t new_dir, uint8_t speed)
{
    switch(new_dir){
      case RIGHT:
        analogWrite(IN1, speed);
        analogWrite(IN2, 0);
        analogWrite(IN3, 0);
        analogWrite(IN4, 0);
        break;
      case LEFT:
        analogWrite(IN1, 0);
        analogWrite(IN2, 0);
        analogWrite(IN3, speed);
        analogWrite(IN4, 0);
        break;
      case FWD:
        analogWrite(IN1, speed);
        analogWrite(IN2, 0);
        analogWrite(IN3, speed);
        analogWrite(IN4, 0);
        break;
      case SOFT_RIGHT:
        analogWrite(IN1, speed);
        analogWrite(IN2, 0);
        analogWrite(IN3, speed - 130);
        analogWrite(IN4, 0);
        break;
      case SOFT_LEFT:
        analogWrite(IN1, speed- 130);
        analogWrite(IN2, 0);
        analogWrite(IN3, speed);
        analogWrite(IN4, 0);
        break;

    }
}
