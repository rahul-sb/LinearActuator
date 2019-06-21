/*
	Linear_actuator.h - Library that controls the stepper motor which
	moves the linear actuator (without feedback from encoder). This 
	library builds on Stepper library v(1.2.1) NOT v(1.2.0) or below.

	Created by Rahul Subramonian Bama, June 19, 2019
	GNU GPL License
 */

#include "LinActStepper.h"


namespace ns_act = Actuator::Linear::WithStepper;


ns_act::Obj ns_act::init(const uint8_t (&pins)[4], const uint16_t& num_steps, const uint8_t& lead_length, const uint8_t& micro_steps){

	static uint8_t count = 0;

	ns_act::Obj my_actuator;
	my_actuator.id = count;
	count++;

	if (micro_steps == 1)	{
		my_actuator.stepper_obj = ::Stepper(num_steps, pins[0], pins[1], pins[2], pins[3]);
	} else {
		my_actuator.stepper_obj = ::Stepper(num_steps, true, micro_steps, pins[0], pins[1], pins[2], pins[3]);
	}

	my_actuator.settings.steps_per_rev = num_steps*micro_steps;
	my_actuator.settings.lead_length = lead_length;
	my_actuator.convert.disp2steps = my_actuator.settings.steps_per_rev / static_cast<double>(lead_length);
	my_actuator.printStatus = false;

	return my_actuator;
}


void ns_act::move(Obj& my_actuator, const int32_t& num_steps){

	if (my_actuator.printStatus == true){
		Serial.print("Linear Actuator Stepper #");
		Serial.print(my_actuator.id);
		Serial.print(" >> Move >> Time(millis), No. of Steps: ");
		Serial.print(millis());
		Serial.print(", ");
		Serial.println(num_steps);

		Serial.flush();
	}

	my_actuator.stepper_obj.step(num_steps);
}


void ns_act::move(Obj& my_actuator, const double& relative_disp_mm){
	int32_t num_steps = ns_act::getSteps(my_actuator, relative_disp_mm);

	if (my_actuator.printStatus == true){
		Serial.print("Linear Actuator Stepper #");
		Serial.print(my_actuator.id);
		Serial.print(" >> Move >> Time(millis), Rel. Disp(mm), No. of Steps: ");
		Serial.print(millis());
		Serial.print(", ");
		Serial.print(relative_disp_mm);
		Serial.print(", ");
		Serial.println(num_steps);

		Serial.flush();
	}

	my_actuator.stepper_obj.step(num_steps);
}


void ns_act::setSpeed(Obj& my_actuator, const uint16_t& rpm){

	if (my_actuator.printStatus == true){
		Serial.print("Linear Actuator Stepper #");
		Serial.print(my_actuator.id);
		Serial.print(" >> SetSpeed >> Time(millis), Speed(rpm): ");
		Serial.print(millis());
		Serial.print(", ");
		Serial.println(rpm);

		Serial.flush();
	}

	my_actuator.stepper_obj.setSpeed(rpm);
}


int32_t ns_act::getSteps(const Obj& my_actuator, const double& displacement){
	return displacement * my_actuator.convert.disp2steps;
}


void ns_act::printCommands(Obj& my_actuator, const bool& status){
	my_actuator.printStatus = status;
}
