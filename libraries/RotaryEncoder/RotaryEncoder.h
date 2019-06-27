/*
	Encoder.h - Library that keeps track of encoder postion.

	See example for sample code.

	About Encoder:
	The encoder has 3 channels: A, B and Index.
	
	Channel A and B together are used to determine the direction and the position of
	the encoder. The index channel gives out a pulse whenever the encoder completes
	one full revolution. You may find channels A' and B' in the encoder, these are 
	just the inverse of channels A and B respectively, i.e., if channel A reads 5V,
	then A' reads 0V. Signals from channel A' and B' can be used to filter the 
	signals from channels A and B respectively. This can be used when there is a lot of
	noise in the signals.

	In order to faithfully track the position of the encoder, the channels A and B must be
	connected to the external digital interrupt pins of the micrcontroller. This library
	keeps track of the encoder position using digital interrupts.

	For Arduino UNO only pins 2 and 3 are external interrupt pins.
	See https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

	This code has been inspired from: https://playground.arduino.cc/Main/RotaryEncoders/




	About Code:
	In this code I create an struct object of the encoder which consists of Settings, ConversionFactor 
	and State. In order to get the struct object pass in the required parameters
	(pins[2] and encoder's counts per revolution) as arguments to the "init" function.

	I also maintain separate "pins[2]" and "position" variables (local to this file, hence "static") that helps
	to run ISR function, since I cannot pass arguments to the ISR. 

	Note that I'm also creating just a single pins[2] and position since for each arduino I can use only 1 encoder 
	(because of the availability of external interrupt pins), so I cannot scale the library to add more encoders.

	I update the struct object with the "position" variable using the "update" function. You, the programmer can
	choose to directly use the "pos" variable AFTER calling the update function:
	
	update(Object);
	long position = Object.state.pos;

	OR use the libraries in-built functions like "getPosition", "getAngle", "getRevolutions" to get the updated values:
	long position = getPosition(Object); // This gives the updated value always.

	The libraries in-built "printPosition" and "printAll" also updates the state and then prints via serial.
	
	
	Created by Rahul Subramonian Bama, June 19, 2019
	GNU GPL License
 */


#include "Arduino.h"


#ifndef ROTARYENCODER_H
#define ROTARYENCODER_H

namespace Sensor{
	namespace Encoder{
		namespace Rotary{

			struct Settings	{ 
				uint16_t cpr;	
			};
			
			struct ConversionFactor	{ 
				double pos2angle; 
				double pos2rev; 
			};

			struct State { 
				volatile long pos; 
			};

			typedef struct MyObj{
				// I'm not declaring settings and convert as const because of the extra 
				// code that I have to write as constructor especially to copy arrays and structs.
				// The programmer must promise that he/she won't be changing these values.
				Settings settings; 
				ConversionFactor convert;
				State state;
			} Obj;

			static uint8_t pins[2];	// I'm creating "static" because I cannot pass args to ISR.
			static State state;	    // Also note that I can only use 1 encoder/arduino. 
			
			// I'm returning an Object of encoder so that I can pass it to other systems.
			Obj init(const uint8_t (&pins)[2], const uint16_t& cpr); // Send external interrupt pins and 
																	 // counts per revolution of encoder as args
			void update(Obj& my_rotary); // You MUST call this function to get the most updated values if you are
										 // directly reading the Obj values like: my_rotary.state.pos; else
										 // if you are using getPosition or getAngle or getRevolutions, it automatically
										 // updates the Obj and gives you the updated values.

			long getPosition(Obj& my_rotary); // Updates and sends current position of encoder in counts
			double getAngle(Obj& my_rotary);  // Updates and sends counts of encoder converted to angles in degrees not radians
			double getRevolutions(Obj& my_rotary); // Updates and sends counts in num of revolutions: Counts/CPR
			double getRevolutions(Obj& my_rotary, const long& position); // Gives num of revolutions for a given position

			void printPosition(Obj& my_rotary); // Updates and prints time and position via serial
			void printAll(Obj& my_rotary);     // Updates and prints time, position, angle and revolutions via serial
		}		
	}
}


// Encoder digital interrupt functions
static void doEncoderChannelA();	// They are declared outside the namespace because ISR needs the function to be global
static void doEncoderChannelB();	// ISR: Interrupt Service Routine.


// Set the namespace as library name so that it is easier to access the functions
namespace RotaryEncoder = Sensor::Encoder::Rotary;

#endif
