/*
	This code that moves the linear actuator to a target position 
	with encoder feedback.

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
#include "LinActStepper.h"
#include "LinActWithRotEnc.h"
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


// System Settings
static const float TOLERANCE_FACTOR = 2.5; // This gets multipled by CPR/(NUM_STEPS * MICRO_STEPS)
						// This value is used as an upper and lower bound for the targeted encoder value to reach. 


// Shorthand notation for namespace.
namespace ns_sys = LinActWithRotEnc;
namespace ns_rot = RotaryEncoder;
namespace ns_act = LinActStepper;


// Get objects for Encoder and Linear actuator
static ns_rot::Obj my_rotary   = ns_rot::init(ENCODER_PINS, CPR);
static ns_act::Obj my_actuator = ns_act::init(STEPPER_PINS, NUM_STEPS, LEAD_LENGTH, MICRO_STEPS);
static ns_sys::Obj my_system   = ns_sys::init(my_rotary, my_actuator, TOLERANCE_FACTOR);
// Note: You can create any number of actuators and create any number of systems based on encoders and actuators.
//       But you can only use 1 encoder/arduino because of the lack of external interrupt pins.



void setup(){
	ns_act::printCommands(my_actuator, true);

	// Start communication and wait for user i/p
	Serial.begin(SERIAL_BAUD_RATE);
	Serial.println("Enter Speed (in rpm): ");
	SerialComm::waitForSignal();

	long speed = Serial.parseInt();
	Serial.read();	// Clear i/p buffer
	ns_act::setSpeed(my_actuator, speed);
	ns_rot::printAll(my_rotary);
	delay(1);
}

void loop(){
	
	Serial.println("Enter absolute distance (from starting position) in mm: ");
	SerialComm::waitForSignal();

	float displacement = Serial.parseFloat();
	Serial.read();	// Clear i/p buffer
	ns_sys::moveTo(my_system, displacement);
  
	ns_rot::printAll(my_rotary);
	delay(1);
}
