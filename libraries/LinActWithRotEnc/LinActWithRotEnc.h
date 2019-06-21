/*
	LinearActuatorWithFeedback.h - Library that controls the 
	stepper motor which moves the linear actuator with encoder feedback.
	This library builds on "LinearAcuator" and "RotaryEncoder" libraries.

	How feedback is implemented:
	In addition to passing the arguments for the Rotary Encoder and the
	Linear Actuator in the "init" function, a "tolerance_factor" is also 
	passed. This factor multiples with the 
	(Encoder's Counts per rev)/(Stepper's num of steps per rev including microsteps)	
	to get the Limit(L). The stepper motor stops rotating when encoder position is 
	within the range of (+L,-L) of target_encoder_postion. The target_encoder_postion
	is calculated automatically from the ablosute postion that is specified in the "moveTo"
	function.

	

	About Code:
	Similar style as in RotaryEncoder.h. But without the need for any static variables. You can create as many
	systems that consists of actuator and encoder as you want (depending upon the number of pins available and 
	based on 1 encoder). See "RotaryEncoder.h" for more details.


	Created by Rahul Subramonian Bama, June 19, 2019
	GNU GPL License
 */


#include "RotaryEncoder.h"
#include "LinActStepper.h"


#ifndef LINACTWITHROTENC_H
#define LINACTWITHROTENC_H

namespace System{
	namespace StepperRotary{

		struct Constraint{ 
			uint16_t encpos_tolerance; // The stepper motor stops when it is within this tolerance of encoder target position.
		}; 
		
		struct ConversionFactor{ 
			double disp2encpos; // Calculates encoder position from the given displacement
			double encpos2steps; 
		};

		typedef struct MyObj{
			Constraint constraint;
			ConversionFactor convert;

			// Pointers to store the address of the rotary and actuator combo
			RotaryEncoder::Obj* pRotary;
			LinActStepper::Obj* pActuator;
		} Obj;


		// Send the encoder settings, the linear actuator settings (see Linear Actuator library) 
		// and tolerance factor - the factor by which enc_pos_tolerance is multiplied by.
		Obj init(RotaryEncoder::Obj& my_rotary, LinActStepper::Obj& my_actuator, const float& tolerance_factor);
		void moveTo(Obj& my_system, const double& absolute_disp_mm); // Move to the absolute displacement in mm
	}	
}


// Set the namespace as library name so that it is easier to access the functions
namespace LinActWithRotEnc = System::StepperRotary;


#endif
