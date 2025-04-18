/*
 *  Copyright (C) 2002-2012  The DOSBox Team
 *  Copyright (C) 2013-2014  bjt, elianda
 *  Copyright (C) 2015-2025  ab0tj
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef EXPORT_H

#include <stdbool.h>

#define NULL 0 /* SOFTMPU */
#define RTCFREQ 4000 /* SOFTMPU: RTC interrupt frequency */

typedef unsigned char Bit8u;
typedef unsigned int Bitu;

typedef enum EventID {MPU_EVENT,RESET_DONE,EOI_HANDLER,NUM_EVENTS} EventID;

/* Interface functions */
void MPU401_Init();
void MPU401_WriteCommand(Bit8u val);
Bit8u MPU401_ReadData(void);
void MPU401_WriteData(Bit8u val);
Bit8u QueueUsed();
void send_midi_byte();
void PIC_Update();

#define EXPORT_H
#endif
