#include <stdio.h>

/* States */
typedef void *(*statefunc)();

void *move_on();
void *move_off();

/* State pointer */
void (*statefunc)() = move_on;

void move_on(){
  // do something
  return move_off;
}

void move_off() {
  // do something
  return  move_on;
}
