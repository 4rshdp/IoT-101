// file     LispMotorDemo.ino
// by lisper (leyapin@gmail.com)
// provide some useful function make it easy to control car with DC motors use L298x
// Copyright (c) 2016 Lisper.li
// MIT License(索性将其开源出来)

#include <LispMotor.h>

//left1=2 left2=4 right1=7 right2=8 leftEn=3 rightEn=5

LispMotor motor (2,4,7,8,3,5);

void setup () {
}

void loop () {
	motor.control(1, 50, 2, 0);
	delay (2000);
	motor.control(50, 50);
	delay (2000);
	motor.control(0, 0);
	delay (200);
	motor.control(-50, -50);
	delay (2000);
	motor.control(0, 0);
	delay (200);
}

