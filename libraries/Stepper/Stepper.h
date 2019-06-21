/*
 * Stepper.h - Stepper library for Wiring/Arduino - Version 1.2.1
 *
 * Original library        (0.1)   by Tom Igoe.
 * Two-wire modifications  (0.2)   by Sebastian Gassner
 * Combination version     (0.3)   by Tom Igoe and David Mellis
 * Bug fix for four-wire   (0.4)   by Tom Igoe, bug fix from Noah Shibley
 * High-speed stepping mod         by Eugene Kozlenko
 * Timer rollover fix              by Eugene Kozlenko
 * Five phase five wire    (1.1.0) by Ryan Orendorff
 * Microstepping on bipolar(1.2.0) by Attila Kov�cs
 * Few corrections         (1.2.1) by Rahul Subramonian Bama
 * 
 * v(1.2.1) Corrections Include: 
 * 1. Commenting out analogWriteFreq();
 * 2. Setting PWMRANGE to 255
 * 3. Adding an empty constructor as place holder.
 * 4. Reducing the overall size of the class by converting
 *    unncessary 'ints'. Reducing size in RAM by 32 bytes.
 * 5. Changed the relavant functions pass by value to pass by reference,
 *    to speed up runtime execution of the function in arduino.
 *    
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Drives a unipolar, bipolar, or five phase stepper motor.
 *
 * When wiring multiple stepper motors to a microcontroller, you quickly run
 * out of output pins, with each motor requiring 4 connections.
 *
 * By making use of the fact that at any time two of the four motor coils are
 * the inverse of the other two, the number of control connections can be
 * reduced from 4 to 2 for the unipolar and bipolar motors.
 *
 * A slightly modified circuit around a Darlington transistor array or an
 * L293 H-bridge connects to only 2 microcontroler pins, inverts the signals
 * received, and delivers the 4 (2 plus 2 inverted ones) output signals
 * required for driving a stepper motor. Similarly the Arduino motor shields
 * 2 direction pins may be used.
 *
 * The sequence of control signals for 5 phase, 5 control wires is as follows:
 *
 * Step C0 C1 C2 C3 C4
 *    1  0  1  1  0  1
 *    2  0  1  0  0  1
 *    3  0  1  0  1  1
 *    4  0  1  0  1  0
 *    5  1  1  0  1  0
 *    6  1  0  0  1  0
 *    7  1  0  1  1  0
 *    8  1  0  1  0  0
 *    9  1  0  1  0  1
 *   10  0  0  1  0  1
 *
 * The sequence of control signals for 4 control wires is as follows:
 *
 * Step C0 C1 C2 C3
 *    1  1  0  1  0
 *    2  0  1  1  0
 *    3  0  1  0  1
 *    4  1  0  0  1
 *
 * The sequence of controls signals for 2 control wires is as follows
 * (columns C1 and C2 from above):
 *
 * Step C0 C1
 *    1  0  1
 *    2  1  1
 *    3  1  0
 *    4  0  0
 *
 * The circuits can be found at
 *
 * http://www.arduino.cc/en/Reference/Stepper
 */

// ensure this library description is only included once
#ifndef Stepper_h
#define Stepper_h

#define PWMRANGE 255
// library interface description
class Stepper {
  public:
    // constructors:
    
    Stepper();

    Stepper(const uint16_t& number_of_steps, const uint8_t& motor_pin_1, const uint8_t& motor_pin_2);

    Stepper(const uint16_t& number_of_steps, bool micro_stepping, const uint8_t& micro_steps, const uint8_t& motor_pin_1, const uint8_t& motor_pin_2, const uint8_t& motor_pwm_pin_1, const uint8_t& motor_pwm_pin_2);
    
    Stepper(const uint16_t& number_of_steps, const uint8_t& motor_pin_1, const uint8_t& motor_pin_2, const uint8_t& motor_pin_3, const uint8_t& motor_pin_4);

	Stepper(const uint16_t& number_of_steps, bool micro_stepping, const uint8_t& number_of_micro_steps, const uint8_t& motor_pin_1, const uint8_t& motor_pin_2, const uint8_t& motor_pin_3, const uint8_t& motor_pin_4, const uint8_t& motor_pwm_pin_1, const uint8_t& motor_pwm_pin_2);
    
    Stepper(const uint16_t& number_of_steps, const uint8_t& motor_pin_1, const uint8_t& motor_pin_2, const uint8_t& motor_pin_3, const uint8_t& motor_pin_4, const uint8_t& motor_pin_5);

    // speed setter method:
    void setSpeed(const uint16_t& whatSpeed);

    // mover method:
    void step(const int& number_of_steps);

	// turns off the coils of the motor
	void off();

    int version(void);

  private:
    void stepMotor(const int& this_step);
	void microStepMotor(const int& this_step, const int& this_micro_step);

    uint8_t direction;            // Direction of rotation
    unsigned long step_delay; // delay between steps, in ms, based on speed
    uint16_t number_of_steps;      // total number of steps this motor can take
    uint8_t pin_count;            // how many pins are in use.
    int step_number;          // which step the motor is on
	bool micro_stepping{ false };      //is microstepping enabled
    uint8_t number_of_micro_steps;          //holds the number of microsteps
    uint8_t micro_step_number;          // which micro step the motor is on
    unsigned long micro_step_delay; //delay between microsteps

    // motor pin numbers:
    uint8_t motor_pin_1;
    uint8_t motor_pin_2;
    uint8_t motor_pin_3;
    uint8_t motor_pin_4;
    uint8_t motor_pin_5;          // Only 5 phase motor

    uint8_t motor_pwm_pin_1;
    uint8_t motor_pwm_pin_2;

    unsigned long last_step_time; // time stamp in us of when the last step was taken

	static int const microstepping_1_2 [4][2][2];

	static int const microstepping_1_4 [4][4][2];

	static int const microstepping_1_8 [4][8][2];
};


#endif

