/*
	This code prints out the encoder position, angle and
	no. of revolutions via Serial communication.

	I tested this code with the following setup:
	
	A Linear actuator (ET-100-22 Newmark ETrack series)  with a standard stepper
	motor setup is used. This actuator has an encoder pre-installed.

	A Cytron 2A Motor Driver Shield is used with Arduino Uno in order to control
	the stepper motor. This shield uses L298P IC. The stepper is connected in a
	bi-polar configuration and "Signed Magnitude" mode is selected on the shield.
	The driver is powered externally via a 5V power supply.
	

	Created by Rahul Subramonian Bama, May 14, 2019
	GNU GPL License
 */


#include "RotaryEncoder.h"
#include "SerialComm.h"


// Serial Settings
static const uint32_t SERIAL_BAUD_RATE = 2000000;


// Encoder Settings
static const uint8_t ENCODER_PINS[2] = {2,3}; // Channel A and B. Connect to External Interrupt pins
static const uint16_t CPR = 4000; // Encoder's counts per revolution

static RotaryEncoder::Obj my_rotary = RotaryEncoder::init(ENCODER_PINS, CPR); // Initialize encoders

void setup(){
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.println("Send any value to start printing encoder values.");
	SerialComm::waitForSignal(); //Specify any value to start data collection
	Serial.read(); // Clear serial buffer
}



void loop(){
	RotaryEncoder::printAll(my_rotary);
	delay(1);
}
