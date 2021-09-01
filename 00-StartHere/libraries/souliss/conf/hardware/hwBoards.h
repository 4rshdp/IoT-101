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
/**************************************************************************/
#ifndef HWBOARDS_H
#define HWBOARDS_H

/**************************************************************************/
/*!
	Select the microcontroller where Souliss for which Souliss have to be
	compiled. This assume that your IDE contains the relevant cores.
	        
		Value       Media
        0x00        No selection (Default)
		0x01		Atmel AVR ATmega
        0x02        Expressif ESP8266
 
*/
/**************************************************************************/
#ifndef MCU_TYPE_INSKETCH
#	define	MCU_TYPE		0x00
#endif

/**************************************************************************/
/*!
	Select the used board type, this selection define the layout of the
	auxiliary pins for the used microcontroller board.
	        
		Value       Media
        0x00        No selection (Default)
		0x01		Freaklabs Chibiduino
        0x02        Arduino Boards
		0x03		KMTronic DINo v1
		0x04		Olimex AVR-T32U4
		0x05		Olimex OLIMEXINO-32U4 
		0x06		Olimex OLIMEXINO-328
		0x07		DFRobots XBoard Relay
		0x08		KMP Electronics DINo v2
		0x09		ExpressIf ESP8266
		0x0A		Moteino RFM69
		0x0B		Authometion LYT8266 Led Bulb
		0x0C		Controllino Maxi (custom CS for W5100 only)
		0x0D		Industruino PLC
		0x0F		KMP Electronics DINo Wroom (ESP8266)
 
*/
/**************************************************************************/
#ifndef BOARD_MODEL_INSKETCH
#	define	BOARD_MODEL			0x00
#endif

/**************************************************************************/
/*!
	Select the used communication module, this setting define the pins layout
	according to the selected board model. Settings of proper vNet media and
	communication interface is further required to load needed drivers.
	        
		Value       Media
        0x00        No selection (Default)
		0x01		Arduino Ethernet Shield with Wiznet W5x00
		0x02		Ethernet Shield with Microchip ENC28J60
		0x03		WiFi Shield with Microchip MRF24WB0MA
		0x04		Olimex MOD-ENC28J60 (UEXT) with Microchip ENC28J60
		0x05		Olimex MOD-WIFI (UEXT) with Microchip MRF24WB0MA
		0x06		Nordic nRF24L01 and nRF24L01+
		0x07		Not used
		0x08		HopeRF RFM69CW/RFM69HCW
		0x09		Olimex MOD-RS485
*/
/**************************************************************************/
#ifndef COMMS_MODEL_INSKETCH
#	define	COMMS_MODEL			0x00
#endif

/**************************************************************************/
/*!
	Select the used (if any) I/O extension board, it include drivers and
	methods for selected device. Not all extension boards and shield need
	this setup.
	        
		Value       Media
        0x00        No selection (Default)
		0x01		Olimex MOD-IO  (UEXT)
		0x02		Olimex MOD-IO2 (UEXT)		
		0x03		Olimex MOD-RGB (UEXT)
		0x04		Authometion LYT88 RGBW Led Bulb
		0x05		Authometion LYT8266 RGBW Led Bulb
		0x06		Iono I/O Board
		0x07		Industruino INDIO I/O Board
		
*/
/**************************************************************************/
#ifndef IOBOARD_MODEL_INSKETCH
#	define	IOBOARD_MODEL		0x00
#endif

#endif