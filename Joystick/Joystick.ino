/*
 Name:		Joystick.ino
 Created:	6/11/2020 3:43:08 PM
 Author:	ipewzner
 I used in some of the code of Darran Hunt and i wish to thank him.
*/

#include "Joystick.h"

Joystick Joy;

void setup()
{
	pinMode(BUTTON_PIN_1, INPUT);
	pinMode(BUTTON_PIN_2, INPUT);
	pinMode(BUTTON_PIN_3, INPUT);
	pinMode(BUTTON_PIN_4, INPUT);	
	
	Serial.begin(115200);
	delay(200);

	Joy.readAxis();
	Joy.clearAllButtons();
}

void loop()
{
	Joy.readButtons();
	Joy.readAxis();
	Joy.sendJoyReport();
	delay(100);
}
