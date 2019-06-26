/* 	
	A Linear actuator (ET-100-22 Newmark ETrack series)  with a standard stepper
	motor setup is used.This actuator has an encoder pre-installed.
	
	This setup (and code) is used to measure electrical response of an elastic piezoresistive
	sensor for a given displacement waveform. 
	
	The encoders of the linear actuator are connected to digital interrupt pins of
	arduino uno. The voltage across the sensor (voltage divider circuit) is
	measured  by connecting it to analog pin 0.

	This code keeps track of the encoder position and prints out the time (in micro
	seconds), encoder position and the analog voltage (0-1023) in a single line
	separated by  a space (" "), via Serial at exactly 200 Hz (using Timer 
	interrupts).

	Note: Do not overload interrupts i.e., it is not desired to have an interrupt
	(encoder, for example) to trigger when another interrupt (timer)	is in
	progress. This causes some interrupts to miss.

	In this code I previously put serial commands in timers ISR and this caused the
	timer's ISR quite come time to execute, by that time the encoders were  also
	moving and this triggered encoder interrupts. This caused quite a few missed
	encoder values. This was hard to identify this error, I had to manually cross 
	check the actual distance moved with the one given by this code using a vernier
	caliper. 

	I corrected this issue by putting the serial print commands in the loop() under
	a bool condition. In the timer ISR I just changed the bool value. Because of
	the very short ISR the encoders counts weren't missed. The encoder interrupt
	are  sometimes triggered when printing via serial and this doesn't affect the
	actual value of any variable that is printed. 

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


// Timer Settings. This works only for ATmega328p, as I'm manipulating its registries
static const uint16_t TIMER1_LOAD = 0; // Reset timer to this value when a compare match interrupt is generated
static const uint16_t TIMER1_COMP = 10000; // Set frequency to 200Hz: (1/200)/(8/16e6) 
                                // where 8 is the timer prescaler (See startTimer function to change value),
                                // 200 is the desired freq, and 16e6 is arduino clock freq.
static volatile bool send_data = false; // Send data only when timer instructs


// Shorthand notation for namespace.
namespace ns_rot = RotaryEncoder;


// Get objects for Encoder
static ns_rot::Obj my_rotary   = ns_rot::init(ENCODER_PINS, CPR);


// Begin Communication, wait for signal and then start timer after i/p received.
void setup(){

	Serial.begin(SERIAL_BAUD_RATE);
	SerialComm::waitForSignal(); //Specify any value to start data collection
	Serial.read(); // Clear serial buffer

	startTimer();
}


// Print when timer instructs
void loop(){

 	if (send_data){
		Serial.print(micros());
		Serial.print(" ");
		Serial.print(ns_rot::getPosition(my_rotary));
		Serial.print(" ");
		Serial.println(analogRead(0));

		send_data = false;
 	}
}




// Supporting Functions:

void startTimer(){
	// Reset timer 1 control Reg A
	TCCR1A = 0;

	// Set timer prescaler to 8
	TCCR1B &= ~(1 << CS12);
	TCCR1B |= (1 << CS11);
	TCCR1B &= ~(1 << CS10);

	// Reset Timer1 and set compare value
	TCNT1 = TIMER1_LOAD;
	OCR1A = TIMER1_COMP;

	// Enable Timer1 comapare interrupt
	TIMSK1 = (1 << OCIE1A);

	// Enable global interrupts
	sei(); 
}


// Print commands are not specified inside ISR because of Interrupt Overhead from Encoders
ISR(TIMER1_COMPA_vect){
	// Reset Timer1
	TCNT1 = TIMER1_LOAD;

	// Send data
	send_data = true;	
}
