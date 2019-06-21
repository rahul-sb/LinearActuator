/*
	This code gives general communication functions with PC via Serial
	
	Created by Rahul Subramonian Bama, April 20, 2019
	GNU GPL License
 */

#include "SerialComm.h"

void Communication::MySerial::waitForSignal(){
	// Wait till arduino receives a serial data
	while (Serial)
	{ 
	if (Serial.available()>0)
	    break;    
	}
}