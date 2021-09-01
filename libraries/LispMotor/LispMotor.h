// file     LispMotor.h
// by lisper (leyapin@gmail.com)
// provide some useful function make it easy to control car with DC motors use L298x
// Copyright (c) 2016 Lisper.li
// MIT License(索性将其开源出来)

#ifndef LISP_MOTOR_H 
#define LISP_MOTOR_H 

#include <Arduino.h>

class LispMotor {
	private:
		uint8_t _left1;
		uint8_t _left2;
		uint8_t _right1;
		uint8_t _right2;
		uint8_t _left_en;
		uint8_t _right_en;
		bool _left_dir;
		bool _right_dir;
	public:
		LispMotor(uint8_t left1, uint8_t left2, uint8_t right1, uint8_t right2, uint8_t left_en, uint8_t right_en);
		void exchangeDir(bool left, bool right);
		void control(int16_t left, int16_t right);
		void control(int8_t leftDir, uint8_t left, int8_t rightDir, uint8_t right);
		void brake ();

};

#endif
