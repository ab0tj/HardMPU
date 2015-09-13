/*
 *  Copyright (C) 2002-2012  The DOSBox Team
 *  Copyright (C) 2013-2014  bjt
 *  Copyright (C) 2015		 ab0tj
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

/*
 * --------------------------------------------
 * HardMPU by ab0tj - Hardware MPU-401 Emulator
 * --------------------------------------------
 * 
 * Based on pic.c from SoftMPU by bjt which was
 * based on original pic.c from DOSBox
 *
 */

/* SOFTMPU: Moved exported functions & types to header */
#include "export.h"

// HardMPU includes
#include "config.h"
#include <util/delay.h>
#include <avr/interrupt.h>

void MPU401_Event(void);
void MPU401_ResetDone(void);
void MPU401_EOIHandler(void);

/* SOFTMPU: Event countdown timers */
static Bitu event_countdown[NUM_EVENTS];
extern Bitu MIDI_sysex_delay; /* SOFTMPU: Initialised in midi.c */

void PIC_AddEvent(EventID event, Bitu delay)
{
        /* Dispatch event immediately on zero delay */
        /* Watch out for blocking loops here... */
        if (delay==0)
        {
                switch (event)
                {
                case MPU_EVENT:
                        /* Don't dispatch immediately as we'll enter an
                        infinite loop if tempo is high enough */
                        delay=1; /* Enforce minimum delay */
                        break;
                case RESET_DONE:
                        MPU401_ResetDone();
                        break;
                case EOI_HANDLER:
                        MPU401_EOIHandler();
                        break;
                default:
                        break;
                }
        }

        /* SOFTMPU: Set the countdown timer */
        event_countdown[event]=delay;
}

void PIC_RemoveEvents(EventID event)
{
        /* SOFTMPU: Zero the countdown timer (disable event) */
        event_countdown[event]=0;
}

void PIC_Init(void)
{
        Bitu i;

        /* SOFTMPU: Zero countdown timers */
        for (i=0;i<NUM_EVENTS;i++)
        {
                PIC_RemoveEvents(i);
        }
}

//void PIC_Update(bool blocking)
ISR(TIMER1_COMPA_vect)
{
        Bitu i;

        /* if (blocking)
        {
                _delay_us(250);
        } */

        /* SOFTMPU: Decrement sysex delay used in midi.c */
        if (MIDI_sysex_delay > 0)
        {
                MIDI_sysex_delay--;
        }

        /* SOFTMPU: Decrement countdown timers and dispatch as needed */
        for (i=0;i<NUM_EVENTS;i++)
        {
                if (event_countdown[i] > 0)
                {
                        event_countdown[i]--;

                        if (event_countdown[i]==0)
                        {
                                /* Dispatch */
                                switch (i)
                                {
                                        case MPU_EVENT:
                                                MPU401_Event();
                                                break;
                                        case RESET_DONE:
                                                MPU401_ResetDone();
                                                break;
                                        case EOI_HANDLER:
                                                MPU401_EOIHandler();
                                                break;
                                        default:
                                                break;
                                }
                        }
                }
        }
}