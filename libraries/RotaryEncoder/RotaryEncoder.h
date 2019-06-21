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
	



	Structuring this Code:
	I purpose of getting a struct object from the "init" function is preferred because I wanted to pass this struct
	to other systems (libraries) that build upon this library easily so that I can easily build systems using this approach.
	For example, if I have a LinearActuator library in a similar way, then I can create a Linear-actuator with encoder feedback
	system easily using just using the objects. 

	I also avoided creating classes because it is very easy to get confused as to how messages travel within the class especailly
	while inheriting you have to mentally keep track of whether or not you can call/access a function/variable because it was 
	declared as private and then later change the function to "private" or "public" and get frustrated. The other issue in using 
	the classes is that it can get too complicated too easily jus after a few inheritances and the major issue is that as said 
	before you have to keep in mind all these different combinations that can occur in your mind and then program accordingly. 
	Which I think could be completely avoided using this approach. 
	
	The main focus of programming should be Clarity. Everything must be clearly stated/defined so that it is easily readable
	like a story.

	I created a simple struct that tells the programmer "Clearly" whats all that is available and what all can be called as
	functions. Also by declaring the struct all my data is in one place. If I need the user not call a function by mistake I 
	will either put those functions in a different namespace or declare them as static.

	If I wanted to do class-like inheritance, then I create whatever variables are required as a new struct and then create
	separate functions that work on the newly created struct. If the class is just an incremental upgrade from the existing 
	class, then I can just add the small subset of variables that are required to the existing struct and then the relevant 
	functions either in a new namespace or in the same namespace depending on the situation.

	I also created functions that take in the struct object as arguments so that I can choose to operate on the 
	enitre set of data (which is neatly organized in sequence by the struct). There are trade-offs to this approach as a programmer
	can easily change a value in the struct without calling any functions. I design the code such that whatever that I need to 
	change in the struct object I do it using functions. This is the one thing that I have to keep in mind while writing other libraries
	that build on this library. There may be other trade-offs which I'm not yet aware of. I'll update this library when I find loop-holes 
	in this	approach and when I remember that I have to update this library. Until then, have fun with this library!



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