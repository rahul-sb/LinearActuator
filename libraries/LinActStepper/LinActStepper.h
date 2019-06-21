/*
	Linear_actuator.h - Library that controls the stepper motor which
	moves the linear actuator (without feedback from encoder). This 
	library builds on Stepper library v(1.2.1) NOT v(1.2.0) or below.
	
	This library can only be used to control a bi-polar stepper motor with
	or without microstepping. To make accomodations for other configurations
	make changes in "init" function.

	Note: 
	Order of pins: 	 
	The sequence of pins is the same as that for the Stepper library.
	Note that for the microstepping the last two pins MUST be able perform 
	AnalogWrite(),  i.e., generate a PWM wave. 

	Uni-polar vs Bi-polar
	Depending upon the torque requirement and the availability of 
	pins you can select between uni-polar wiring or bi-polar wiring
	of stepper motor. See link for more details:
	https://www.petervis.com/dictionary-of-digital-terms/unipolar-vs-bipolar-stepper/unipolar-vs-bipolar-stepper.html

	Lead Length vs Pitch:
	While providing lead_length in "init" fn, make sure that you
	specify the lead length and NOT the pitch. They are the same for
	single start threads but not the same when the lead screw is multi-
	start. See: https://www.youtube.com/watch?v=SK_PpWN296U
	For example, for double-start thread: lead_length = 2*pitch. 	
	

	About Code:
	Similar style as in RotaryEncoder.h. But without the need for any static variables because you can control as many
	actuators as you want (depending upon the number of pins available). See "RotaryEncoder.h" for more details.


	Created by Rahul Subramonian Bama, June 19, 2019
	GNU GPL License
 */


#include "Arduino.h"
#include "Stepper.h"


#ifndef LINACTSTEPPER_H
#define LINACTSTEPPER_H

namespace Actuator {
	namespace Linear {
		namespace WithStepper {

			struct Settings{ 
				uint8_t lead_length; 
				uint16_t steps_per_rev; 
			};
			
			struct ConversionFactor{ 
				double disp2steps; 
			};

			typedef struct MyObj{
				uint8_t id; // This is an unique ID for each stepper motor that you create
				Settings settings;
				ConversionFactor convert;
				::Stepper stepper_obj;
				bool printStatus; // Set this to true or false via "printCommands", to print the commands broadcasted. Default: False
			} Obj;


			// Send the stepper motor pins according to the stepper library; the number of steps the stepper takes to complete one
			// revolution; the lead length of the lead screw NOT the pitch in mm, see: https://www.youtube.com/watch?v=SK_PpWN296U; 
			// and finally the number of micro steps (see stepper library) this value can be 1,2,4 or 8. 1 means NO microstepping.
			Obj init(const uint8_t (&pins)[4], const uint16_t& num_steps, const uint8_t& lead_length, const uint8_t& micro_steps);
			// Note: The Num. of steps per revolution is multiplied by micro_steps

			// You can give move with either displacement in mm or just directly num of steps.
			void move(Obj& my_actuator, const int32_t& num_steps); // Move by relative(from current position) num of steps
			void move(Obj& my_actuator, const double& relative_disp_mm);	// Move by the relative displacement in mm
			void setSpeed(Obj& my_actuator, const uint16_t& rpm); // RPM at which the motor moves, see stepper library
			
			int32_t getSteps(const Obj& my_actuator, const double& displacement); // Gets the number of steps required for the given displacement
			void printCommands(Obj& my_actuator, const bool& status); // Prints to serial all the commands broadcasted to linear actuator
		}		
	}
}


// Set the namespace as library name so that it is easier to access the functions
namespace LinActStepper = Actuator::Linear::WithStepper;

#endif