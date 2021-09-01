/**************************************************************************
	Souliss 
    Copyright (C) 2013  Veseo

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
	Originally developed by Dario Di Maio
	
***************************************************************************/
/*!
    \file 
    \ingroup
*/

#ifndef MODRGB_H
#define MODRGB_H

#include "tools/types.h"
#include "tools/I2C/mcu_avr/I2C.h"
#include "Olimex.h"

// Functional Codes for I2C Communication to MOD-RGB
#define	STOP_PWM	0x00
#define	START_PWM	0x01
#define	SET_RGB		0x03

void mRGB_Init();
void mRGB_Reset();
void mRGB_SetColor(uint8_t *color);

#endif
