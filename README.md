# Linear Actuator with Stepper Motor and Encoder

A linear actuator is a device that converts rotary motion to motion along a straight line. In this example project demonstrating the library, a stepper motor is used to produce a rotary motion and an incremental [rotary encoder](https://howtomechatronics.com/tutorials/arduino/rotary-encoder-works-use-arduino/) is mounted along the shaft of the stepper motor. The rotary motion to linear motion is converted using a lead-screw mechanism.

## Setup Used

A Linear actuator (ET-100-22 Newmark ETrack series)  with a standard 2-phase stepper motor setup is used. This actuator has an encoder pre-installed.

A Cytron 2A Motor Driver Shield is used with Arduino Uno to control the stepper motor. This shield uses the L298P IC.

Note: With the L298P IC, you can only control a 2-phase stepper motor. If you need to control a 5-phase stepper motor, you have to look for other options. Also, the library is not compatible with a 5-phase stepper motor.

## Electrical Connection

The stepper is connected in a [bipolar](https://www.petervis.com/dictionary-of-digital-terms/unipolar-vs-bipolar-stepper/unipolar-vs-bipolar-stepper.html) configuration and "Signed Magnitude" mode is selected on the shield. The motor driver is powered externally via a 5V power supply.

Since this is a 2-phase stepper motor, each phase is controlled separately by each bridge in the IC. If you want to control the motors using [micro-stepping](https://www.rs-online.com/designspark/stepper-motors-and-drives-what-is-full-step-half-step-and-microstepping), then make sure to connect one PWM pin of Arduino for each of the bridge. Make a note of how the pins from the motor driver connect to the pins of Arduino. In the code, the order of pins will be {Motor1, Motor2, Motor1_pwm, Motor2_pwm}.

For this setup, PhaseA, PhaseB, PhaseA' and PhaseB' of the stepper motor (see page 6 in linear actuator datasheet) are connected to Motor1_A, Motor2_A, Motor1_B, Motor2_B pins of the motor driver (shield) respectively. These pins are then connected to pins D7, D4, D6 and D5 of Arduino respectively via the shield. As shown below *insert image*

![1](https://drive.google.com/uc?export=view&id=1-JS5EMniZpB2Bh1aENQqIyiJNSOLmbOs = 250x250)
![2](https://drive.google.com/uc?export=view&id=1-KmMvKml32Ct5qEtYEO2Epu3FkqS5cpq = 250x250)
![3](https://drive.google.com/uc?export=view&id=1-SHnmil2_GmCKVE5sH950cd2E6Eb63uF = 250x250)

*See page 6 in linear actuator datasheet for which pin legend for DB-9 and HD-15 cables* 

For the encoder, the channelA and channelB pins (see page 6 in linear actuator datasheet) are connected to pins D2 and D3 of Arduino (these are external interrupt pins). By switching the D2 and D3 pins the clockwise and counterclockwise direction of rotation gets switched. You can connect it whichever way you feel is necessary. The encoder is powered using the Arduino board itself.

Note: The channelA and channelB pins of the encoder MUST be connected to external interrupt pins of Arduino since I'm using an external interrupt to count the signals from the encoder.


## About the code:

I've named the examples according to what you want to do with the setup. I've also given a short description in the corresponding example files on the code.

*New!*: In the projects folder, I've uploaded the code (which I run on 2 different Arduinos) that I use to test the elastomer-nanocarbon composite piezoresistive sensors that I fabricate. I've also written some learning outcomes and design considerations that I made while structuring the code in the code itself.


## About Structuring Libraries:

I took a [Functional Programming](http://blog.jenkster.com/2015/12/what-is-functional-programming.html) (FP) approach while I was creating this library. Mainly because I wanted to keep the data that I'm manipulating clear to myself and to the user who will be reading this code later. 

The main reason why I avoided the Object Oriented Programming (OOP) approach is because of the confusion on how the data gets manipulated just after creating a few inheritances. To avoid this confusion, I have to keep in mind the entire structure of the class, and it's dependencies while programming, which is a lot of lines of code that I have to read and understand. 

Another problem with multiple inheritances, with OOP, is that you have to keep in mind whether or not you can access to a particular function in the derived class. You also have to do a lot of planning at the start while writing the base class to make sure you capture the entire gamut of the class. It's tough in the beginning to foresee the entire scope where the base class could be used, and you may end up wasting a lot of time in making unnecessary decisions in structuring the code.

I would prefer an approach where I can add new features easily without giving much thought about the base class, the type of inheritance, and the structure of the rest of the code. In my attempt to find different ways of structuring my code, this is one of them. 

In this library, I packaged all the necessary variables that are required in a single nested structure and put this structure and the functions that manipulate this structure in a unique namespace. At the end of the header file declaration, I declared a namespace variable (having the same name as the header file) which stores this unique namespace. For example, for initializing two libraries A and B, the following approach can be taken:
"A::Obj my_obj = A::init(args)" or "B::Obj my_obj = B::init(args)". This structure is followed to bring some standard to the libraries that I'm creating. 

If I wanted to create a small improvement of some library, depending on the requirement, I could just modify the nested structure to accommodate more variables or put the new variables, and it's corresponding functions in a new namespace. 

Following the FP approach, I have more control and clarity over the main "thread" (sequence of execution/data manipulation) whereas, in the OOP approach, it's not always clear. I prefer to have this control and clarity while I'm coding, which saves me a lot of time. 



## Short intro to encoders and linear actuators:

See description written in "RotaryEncoder.h" and "LinActStepper.h" if you want to brush up on encoders and linear actuators quickly. Have a look at the description in "LinActWithRotEnc.h" on how the feedback is implemented. 

I've also given a description about what to do when you are using a single/multi-start thread as lead screw, etc. in the header file description itself. 






