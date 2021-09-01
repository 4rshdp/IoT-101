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
#ifndef IODEF_H
#define IODEF_H

#include "GetConfig.h"			// need : hwBoards.h

// Include pins definition for KMTronic DINo v1
#if(BOARD_MODEL == 0x03)						
#	include "hardware/KMPElectronics/DINo_v1.h"
#endif

// Include pins definition for KMP Electronics DINo v2
#if(BOARD_MODEL == 0x08)						
#	include "hardware/KMPElectronics/DINo_v2.h"
#endif

// Include pins definition for DFRobots XBoard Relay
#if(BOARD_MODEL == 0x07)						
#	include "hardware/DFRobots/XBoardRelay.h"
#endif

// Include drivers for Olimex MOD-IO
#if(IOBOARD_MODEL == 0x01)						
#	include "hardware/Olimex/MODIO.h"
#	include "hardware/Olimex/Souliss_MODIO.h"
#endif

// Include drivers for Olimex MOD-IO2
#if(IOBOARD_MODEL == 0x02)						
#	include "hardware/Olimex/MODIO2.h"
#	include "hardware/Olimex/Souliss_MODIO.h"
#endif

// Include drivers for Olimex MOD-RGB
#if(IOBOARD_MODEL == 0x03)						
#	include "hardware/Olimex/MODRGB.h"
#	include "hardware/Olimex/Souliss_MODRGB.h"
#endif

// Include drivers for Authometion LYT88 RGBW Led Bulb
#if(IOBOARD_MODEL == 0x04)						
#	include "hardware/Authometion/LYT88.h"
#endif

// Include drivers for Authometion LYT8266 RGBW Led Bulb
#if(BOARD_MODEL == 0x0B)						
#	include "hardware/Authometion/LYT8266.h"
#endif

// Include defines for Iono I/O Board
#if(IOBOARD_MODEL == 0x06)						
#	include "hardware/Iono/IonoBoard.h"
#endif

// Include pins definition for Controllino Maxi
#if(BOARD_MODEL == 0x0C)						
#	include "hardware/SGTronic/ControllinoMaxi.h"
#endif

// Include defines for Industrino INDIO Board
#if(IOBOARD_MODEL == 0x07)						
#	include "hardware/Industruino/INDIO.h"
#endif

// Include defines for DINo Wroom WiFi Board
#if(BOARD_MODEL == 0x0F)						
#	include "hardware/KMPElectronics/DINoWroom.h"
#endif

#endif
