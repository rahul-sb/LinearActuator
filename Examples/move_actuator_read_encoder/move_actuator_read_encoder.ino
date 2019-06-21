/*
	This code moves the linear actuator to a distance specified by the user
	without any encoder	feedback, and simultaneously prints the encoder values.
	
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

#include "LinActStepper.h"
#include "RotaryEncoder.h"
#include "SerialComm.h"


// Serial Settings
static const uint32_t SERIAL_BAUD_RATE = 2000000;


// Encoder Settings
static const uint8_t ENCODER_PINS[2] = {2,3}; // Channel A and B. Connect to External Interrupt pins
static const uint16_t CPR = 4000; // Encoder's counts per revolution


// Linear Actuator Settings
static const uint8_t STEPPER_PINS[4] = {7,4,6,5};
static const uint16_t NUM_STEPS = 200; // for stepper to complete 1 revolution
static const uint8_t MICRO_STEPS = 8; // Each step is divided into this many steps
static const uint8_t LEAD_LENGTH = 12; // in mm.


// Get objects for Encoder and Linear actuator
static RotaryEncoder::Obj my_rotary = RotaryEncoder::init(ENCODER_PINS, CPR);
static LinActStepper::Obj my_actuator = LinActStepper::init(STEPPER_PINS, NUM_STEPS, LEAD_LENGTH, MICRO_STEPS);



void setup(){
	// Print commands sent to linear actuator
	LinActStepper::printCommands(my_actuator, true);

	// Start communication
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.println("Enter Speed (in rpm): ");
	SerialComm::waitForSignal();

	long set_speed = Serial.parseInt();
 	Serial.read(); // Clear i/p buffer
	LinActStepper::setSpeed(my_actuator, set_speed);
	RotaryEncoder::printAll(my_rotary);
	delay(1);
}

void loop(){
	
	Serial.println("Enter distance to move from current position (in mm): ");
	SerialComm::waitForSignal();

	float displacement = Serial.parseFloat();
	Serial.read(); // Clear i/p buffer
	LinActStepper::move(my_actuator, displacement);

	RotaryEncoder::printAll(my_rotary);
	delay(1);
}
