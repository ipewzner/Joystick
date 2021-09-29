#include "Joystick.h"

Joystick::Joystick()
{
	for (size_t i = 0; i < 3; i++)
	{
		max[i] = -1500;
		min[i] = 1500;
	}	
}

void Joystick::sendJoyReport()
{
	Serial.write((uint8_t*)&report, sizeof(joyReport_t));	
}

void Joystick::setButton(uint8_t button)
{
	uint8_t index = button / 8;
	uint8_t bit = button - 8 * index;
	
	this->report.button[index] |= 1 << bit;			
}

void Joystick::clearButton(uint8_t button)
{
	uint8_t index = button / 8;
	uint8_t bit = button - 8 * index;

	this->report.button[index] &= ~(1 << bit);
}

void Joystick::clearAllButtons()
{
	for (int button = 0; button < 8; button++)
	{
		clearButton(button);
	}
}

void Joystick::readButtons()
{
	clearAllButtons();
	int button = 0;

	bool b1 = !digitalRead(BUTTON_PIN_1);
	bool b2 = !digitalRead(BUTTON_PIN_2);
	bool b3 = !digitalRead(BUTTON_PIN_3);
	bool b4 = !digitalRead(BUTTON_PIN_4);
	
	//Button 5-8
	if (b1 && b2)
	{
		button = 5;
		if (b3)button += 2;
		if (b4)button += 1;
	}
	//Button 1-4
	else
	{
		b1 ? button = 1 : 0;
		b2 ? button = 2 : 0;
		b3 ? button = 3 : 0;
		b4 ? button = 4 : 0;
	}
	
	if (button > 0) setButton(button - 1);
}

int16_t Joystick::calibration(int16_t in, int axis)
{
	float axisValue;			//percentag of axis

	//Adjest the opretion range 
	if (in > max[axis]) max[axis] = in;
	if (in < min[axis]) min[axis] = in;

	if (axis == THROTTLE)
	{
		axisValue = delta(in, min[axis]) / stepSize(max[axis], min[axis]); //0-100%
		if (axisValue < SENSITIVITY) axisValue = 0;
		return (int16_t)(MIN + axisValue * RANGE * 2);
	}
	else
	{	 
		middle[axis] = delta(max[axis], min[axis]) / 2 + min[axis];
		
		// The division is due to the change in the behavior of the axis
		if (in > middle[axis])
		{
			axisValue = delta(in, middle[axis]) / stepSize(max[axis], middle[axis]);
			if (axisValue < SENSITIVITY) axisValue = 0;
		}
		else if (in < middle[axis])
		{
			axisValue = delta(in, middle[axis]) / stepSize(middle[axis], min[axis]);
			if (axisValue > -SENSITIVITY) axisValue = 0;
		}	

		//Turn step to linear data
		return (int16_t)(axisValue * RANGE);
	}
	return (int16_t) 0;
}

void Joystick::readAxis()
{
	int16_t axis[] = { analogRead(AXIS_PIN_1), analogRead(AXIS_PIN_2), analogRead(AXIS_PIN_3) };

	for (size_t i = 0; i < 3; i++)
	{
		this->report.axis[i] = -calibration(axis[i], i);
		this->report.axis[i] = constrain(this->report.axis[i], MIN, MAX);
	}
}

float Joystick::stepSize(int16_t big, int16_t small)
{
	return (float)(big - small) / 100;
}

float Joystick::delta(int16_t big, int16_t small)
{
	return big - small;
}
