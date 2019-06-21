/*
	Encoder.h - Library that keeps track of encoder postion.

	Created by Rahul Subramonian Bama, June 19, 2019
	GNU GPL License
 */

#include "RotaryEncoder.h"

namespace ns_rot = Sensor::Encoder::Rotary;

ns_rot::Obj ns_rot::init(const uint8_t (&pins)[2], const uint16_t& cpr){

  // Set the pins to be External Interrupt
	pinMode(pins[0], INPUT);
	pinMode(pins[1], INPUT);

  digitalWrite(pins[0], HIGH); // turn on pull up resistor. 
	digitalWrite(pins[1], HIGH);

	attachInterrupt(digitalPinToInterrupt(pins[0]), doEncoderChannelA, CHANGE);
	attachInterrupt(digitalPinToInterrupt(pins[1]), doEncoderChannelB, CHANGE);

  // Set static variables
  ns_rot::pins[0] = pins[0];
  ns_rot::pins[1] = pins[1];
  ns_rot::state.pos = 0;

  // Setup the Rotary encoder
  ns_rot::Obj my_rotary;

  my_rotary.state.pos = 0;
  my_rotary.settings.cpr = cpr;
  my_rotary.convert.pos2angle = 360 / static_cast<double>(cpr);
  my_rotary.convert.pos2rev = 1 / static_cast<double>(cpr);

  return my_rotary;
}


void ns_rot::update(ns_rot::Obj& my_rotary){
  my_rotary.state.pos = ns_rot::state.pos;
}


long ns_rot::getPosition(ns_rot::Obj& my_rotary){
  ns_rot::update(my_rotary);
  return my_rotary.state.pos;
}


double ns_rot::getAngle(ns_rot::Obj& my_rotary){
  ns_rot::update(my_rotary);
	return my_rotary.convert.pos2angle * my_rotary.state.pos;
}


double ns_rot::getRevolutions(ns_rot::Obj& my_rotary){
  ns_rot::update(my_rotary);
  return my_rotary.convert.pos2rev * my_rotary.state.pos;
}


double ns_rot::getRevolutions(ns_rot::Obj& my_rotary, const long& position){
  ns_rot::update(my_rotary);
  return my_rotary.convert.pos2rev * position;
}


void ns_rot::printPosition(ns_rot::Obj& my_rotary){
  ns_rot::update(my_rotary);

  Serial.print("Encoder >> Time(millis), Counts: ");
  Serial.print(millis());
  Serial.print(", ");
  Serial.println(my_rotary.state.pos);

  Serial.flush(); // Makes sure that the program proceeds only after printing all statements via serial.
                  // If you don't do this, then you will see that sometimes the statements will not be fully printed
}


void ns_rot::printAll(ns_rot::Obj& my_rotary){
  ns_rot::update(my_rotary);

  Serial.print("Encoder >> Time(millis), Counts, Angle, Revolutions: ");
  Serial.print(millis());
  Serial.print(", ");
  Serial.print(my_rotary.state.pos);
  Serial.print(", ");
  Serial.print(my_rotary.state.pos * my_rotary.convert.pos2angle);
  Serial.print(", ");
  Serial.println(my_rotary.state.pos * my_rotary.convert.pos2rev);

  Serial.flush(); // Makes sure that the program proceeds only after printing all statements via serial.
                  // If you don't do this, then you will see that sometimes the statements will not be fully printed
}


void doEncoderChannelA() {
  /* For channel A, if A and B are both high or both low, it is spinning
     forward. If they're different, it's going backward.
  */
  if (digitalRead(ns_rot::pins[0]) == digitalRead(ns_rot::pins[1])) {
    ns_rot::state.pos++;
  } else {
    ns_rot::state.pos--;
  }
}


void doEncoderChannelB() {
  /* For channel B, if A and B are both high or both low, it is spinning
     backward. If they're different, it's going forward.
  */
  if (digitalRead(ns_rot::pins[0]) == digitalRead(ns_rot::pins[1])) {
    ns_rot::state.pos--;
  } else {
    ns_rot::state.pos++;
  }
}