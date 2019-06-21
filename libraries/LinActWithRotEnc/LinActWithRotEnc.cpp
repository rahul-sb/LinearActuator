/*
	LinearActuatorWithFeedback.h - Library that controls the 
	stepper motor which moves the linear actuator with encoder feedback.
	This library builds on "LinearAcuator" and "RotaryEncoder" libraries.

	Created by Rahul Subramonian Bama, June 19, 2019
	GNU GPL License
 */

#include "LinActWithRotEnc.h"


namespace ns_rot = Sensor::Encoder::Rotary;
namespace ns_act = Actuator::Linear::WithStepper;
namespace ns_sys = System::StepperRotary;


ns_sys::Obj ns_sys::init(RotaryEncoder::Obj& my_rotary, LinActStepper::Obj& my_actuator, 
							   const float& tolerance_factor){

	ns_sys::Obj my_system;

	my_system.constraint.encpos_tolerance = tolerance_factor*my_rotary.settings.cpr/static_cast<double>(my_actuator.settings.steps_per_rev);
	my_system.convert.disp2encpos         = my_rotary.settings.cpr/static_cast<double>(my_actuator.settings.lead_length);
	my_system.convert.encpos2steps        = my_actuator.settings.steps_per_rev/static_cast<double>(my_rotary.settings.cpr);

	my_system.pRotary   = &my_rotary;
	my_system.pActuator = &my_actuator;

	return my_system;
}


void ns_sys::moveTo(ns_sys::Obj& my_system, const double& absolute_disp_mm){

	long target_encpos = absolute_disp_mm * my_system.convert.disp2encpos;

	// Keep moving till the encoder value is reached
	while(abs(target_encpos - ns_rot::getPosition(*my_system.pRotary)) > my_system.constraint.encpos_tolerance){
		int32_t num_steps = (target_encpos - ns_rot::getPosition(*my_system.pRotary)) * my_system.convert.encpos2steps;
		ns_act::move(*my_system.pActuator, num_steps);
	}
}
