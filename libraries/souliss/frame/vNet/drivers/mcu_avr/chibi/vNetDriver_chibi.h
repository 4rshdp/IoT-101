/*******************************************************************
    Copyright (C) 2009 FreakLabs
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of the the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software
       without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

    Originally written by Christopher Wang aka Akiba.
    Please post support questions to the FreakLabs forum.

*******************************************************************/
/**************************************************************************
	Souliss - vNet Virtualized Network

	Modified by Dario Di Maio
	
***************************************************************************/
/*!
    \file 
    \ingroup


*/
/**************************************************************************/
#ifndef VNET_CHIBI_H
#define VNET_CHIBI_H

#include <Arduino.h>
#include "GetConfig.h"				// need : chibiUsrCfg.h

#define BROADCAST_ADDR 0xFFFF

void vNet_Init_M2();
void vNet_SetAddress_M2(uint16_t addr);
uint8_t vNet_Send_M2(uint16_t addr, oFrame *frame, uint8_t len);
uint8_t vNet_DataAvailable_M2();
uint8_t vNet_RetriveData_M2(uint8_t *data);
uint16_t vNet_GetSourceAddress_M2();
uint8_t vNet_GetRSSI_M2();
void vNet_RadioSleep_M2();
void vNet_RadioWakeUp_M2();

#endif
