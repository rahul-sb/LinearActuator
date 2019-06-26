/*
	A Linear actuator (ET-100-22 Newmark ETrack series)  with a standard stepper
	motor setup is used. This actuator has an encoder pre-installed.

	A Cytron 2A Motor Driver Shield is used with Arduino Uno in order to control
	the stepper motor. This shield uses L298P IC. The stepper is connected in a
	bi-polar configuration and "Signed Magnitude" mode is selected on the shield.
	The driver is powered externally via a 5V power supply.

	This code controls the stepper motor to move the linear stage in a predefined
	waveform with encoder feedback. 

	The code gets the rpm of switching the stepper motor, the sensor length and a 
	list of 10 strain percentages as input from the user via serial. This is used
	to set the  speed of the motor and generate the waveforms for each particular
	strain percent according to the sensor length. The genereated waveform is then
	used to control the  stepper with encoder feedback.

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


static float sensor_length;



void setup(){

	// Start communication and wait for user i/p
	Serial.begin(SERIAL_BAUD_RATE);
	SerialComm::waitForSignal();


	// Read the speed, sensor_length and the list of strain %
	long set_speed = Serial.parseInt();
	ns_act::setSpeed(my_actuator, set_speed);

	sensor_length = Serial.parseFloat();

	float percentage_strain[10];
	for (uint8_t i=0; i<10; i++){
		percentage_strain[i] = Serial.parseFloat();
	}


	// Wait to start executing experiment
	SerialComm::waitForSignal();
	Serial.read(); // Clear i/p buffer


	// Execute each strain waveform
	for(uint8_t i=0; i<10; i++){
		if (percentage_strain[i]>0){
			executeWaveform(percentage_strain[i]);
		}
	}
}


void loop(){
}




// Supporting functions:

void executeWaveform(const float& strain){
	// Delay before start of %Strain
	delay(10000);

	// Execute the 3 cycles
	for (uint8_t j=0; j<3; j++){
		cycle(strain);

		// Delay between cycles
		delay(15000);
	}

	// Final 2% strain. Give in absolute values
	strainStep(2.0);
	delay(5000);
	strainStep(0.0);

	// Delay after strain waveform is over
	delay(50000);
}


void cycle(const float& strain){
	// Give Strains percentage in absolute values
	strainStep(strain);
	delay(10000);
	strainStep(strain + 1.0);
	delay(2000);
	strainStep(strain - 1.0);
	delay(2000);
	strainStep(strain);
	delay(5000);
	strainStep(0);

}


void strainStep(const float& strain){
	float abs_disp_mm = -strain*sensor_length/100.0;
	ns_sys::moveTo(my_system, abs_disp_mm);
}
