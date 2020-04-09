/*  Copyright (C) 2015		 ab0tj
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

 /* config.h: AVR specific configuration details */

#ifndef __CONFIG
#define __CONFIG

// pins
#define PIN_IDW		(1<<PORTB3)	// isa output latch CLK
#define PIN_IDR		(1<<PORTB4)	// isa input latch OC
#define PIN_DSR		(1<<PORTB0)	// isa data out flip-flop
#define PIN_DRR		(1<<PORTB1)	// isa data in flip-flop
#define PIN_CRR		(1<<PORTB2)	// isa command in flip-flop;
#define PIN_LED		(1<<PORTC6)	// test LED
#define PIN_TEST	(1<<PORTC7)	// test mode indicator

// parameters
#define F_CPU		20000000UL
#define BAUD_MIDI	39			// 39 = 31250 (MIDI standard)
#define BAUD_UART	39			// 10 = 115200-ish

#endif