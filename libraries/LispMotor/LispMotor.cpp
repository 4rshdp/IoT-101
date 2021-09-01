// file     LispMotor.cpp
// by lisper (leyapin@gmail.com)
// provide some useful function make it easy to control car with DC motors use L298x
// Copyright (c) 2016 Lisper.li
// MIT License(索性将其开源出来)

// dir == true is cw else dir == false is ccw
// leftDir == 1 is cw else leftDir == 2 is ccw

#include <Arduino.h>
#include "LispMotor.h"

LispMotor::LispMotor (uint8_t left1, uint8_t left2, uint8_t right1, uint8_t right2, uint8_t left_en, uint8_t right_en) {
	_left1 = left1;
	_left2 = left2;
	_right1 = right1;
	_right2 = right2;
	_left_en = left_en;
	_right_en = right_en;

	_left_dir = true;
	_right_dir = true;

	pinMode (_left1, OUTPUT);
	pinMode (_left2, OUTPUT);
	pinMode (_right1, OUTPUT);
	pinMode (_right2, OUTPUT);
	pinMode (_left_en, OUTPUT);
	pinMode (_right_en, OUTPUT);
}

void LispMotor::exchangeDir (bool left, bool right) {
	uint8_t temp;
	if (left) {
		temp = _left1;
		_left1 = _left2;
		_left2 = temp;
	}
	if (right) {
		temp = _right1;
		_right1 = _right2;
		_right2 = temp;
	}
}

void LispMotor::control (int16_t left, int16_t right) {
	if (left < 0) {
		left = -left;
		if (_left_dir == true) {
			_left_dir = false;
			digitalWrite (_left1, HIGH);
			digitalWrite (_left2, LOW);
		}
	} else {
		if (_left_dir == false) {
			_left_dir = true;
			digitalWrite (_left1, LOW);
			digitalWrite (_left2, HIGH);
		}
	}

	if (right < 0) {
		right = -right;
		if (_right_dir == true) {
			_right_dir = false;
			digitalWrite (_right1, HIGH);
			digitalWrite (_right2, LOW);
		}
	} else {
		if (_right_dir == false) {
			_right_dir = true;
			digitalWrite (_right1, LOW);
			digitalWrite (_right2, HIGH);
		}
	}

	//if (left > 255) left = 255;
	//if (right > 255) right = 255;

	analogWrite (_left_en, left);
	analogWrite (_right_en, right);
}

void LispMotor::control (int8_t leftDir, uint8_t left, int8_t rightDir, uint8_t right) {
	if (leftDir == 2) {
		if (_left_dir == true) {
			_left_dir = false;
			digitalWrite (_left1, HIGH);
			digitalWrite (_left2, LOW);
		} 
	} else if (leftDir == 1) {
		if (_left_dir == false) {
			_left_dir = true;
			digitalWrite (_left1, LOW);
			digitalWrite (_left2, HIGH);
		}
	}

	if (rightDir == 2) {
		if (_right_dir == true) {
			_right_dir = false;
			digitalWrite (_right1, HIGH);
			digitalWrite (_right2, LOW);
		}
	} else if (rightDir == 1) {
		if (_right_dir == false) {
			_right_dir = true;
			digitalWrite (_right1, LOW);
			digitalWrite (_right2, HIGH);
		}
	}

	analogWrite (_left_en, left);
	analogWrite (_right_en, right);
}

void LispMotor::brake () {
	digitalWrite (_left1, LOW);
	digitalWrite (_left2, LOW);
	digitalWrite (_right1, LOW);
	digitalWrite (_right2, LOW);
}

