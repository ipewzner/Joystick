#pragma once
#include "Arduino.h"
#include "Util.h"

#ifndef JOYSTICK
#define	JOYSTICK

typedef struct joyReport_t {
	int16_t axis[NUM_AXES];
	uint8_t button[(NUM_BUTTONS + 7) / 8]; // 8 buttons per byte
} joyReport;

class Joystick
{

public:
	joyReport report;
	int16_t max[3], min[3], middle[2];

	/**
	 * @brief c-tor.
	 */
	Joystick();

	/**
	 * @brief Turn button on.
	 *
	 * @param uint8_t button.
	 */
	void setButton(uint8_t button);

	/**
	 * @brief Turn off the button.
	 *
	 * @param uint8_t button.
	 */
	void clearButton(uint8_t button);

	/**
	 * @brief Turn off all the buttons.
	 */
	void clearAllButtons(void);

	/**
	 * @brief Send an HID report to the USB interface.
	 */
	void sendJoyReport();

	/**
	 * @brief Adjust the output of the Joystick to be between the desire numbers.
	 *
	 * @param int16_t in.
	 * @param int axle.
	 *
	 * @return int16_t
	 */
	int16_t calibration(int16_t in, int axle);

	/**
	 * @brief read Exis.
	 */
	void readAxis();

	/**
	 * @brief Returns the number of the button that is pressed.
	 */
	void readButtons();

	/**
	 * @brief calculate the size of step.
	 * 	
	 * @param int16_t big.
	 * @param int16_t small.  
	 * 
	 * @return float.
	 */
	float stepSize(int16_t big, int16_t small);
	
	/**
	 * @brief calculate delta.
	 *
	 * @param int16_t big.
	 * @param int16_t small.
	 *
	 * @return float.
	 */
	float delta(int16_t big, int16_t small);

};

#endif // !JOYSTICK

