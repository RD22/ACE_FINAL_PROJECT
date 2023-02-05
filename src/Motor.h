#include "Arduino.h"

/*motor drive*/
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

/*robot interface*/
typedef enum{
  LEFT,
  RIGHT,
  FWD,
  REV,
  SOFT_LEFT,
  SOFT_RIGHT,
}direction_t;

void rbt_stop ();

void rbt_move(direction_t new_dir, uint8_t speed);

void rbt_init();
