/*
	This code gives general communication functions with PC via Serial
	
	Created by Rahul Subramonian Bama, April 20, 2019
	GNU GPL License
 */


#include "Arduino.h"

#ifndef SERIALCOMM_H
#define SERIALCOMM_H

namespace Communication{
	namespace MySerial{
		void waitForSignal();
	}
}

// Set the namespace as library name so that it is easier to access the functions
namespace SerialComm = Communication::MySerial;


#endif

