/*  Copyright (C) 2015-2025		 ab0tj
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

 /* config.h: HardMPU specific configuration details */

#ifndef __CONFIG
#define __CONFIG

#if defined (__AVR_ATmega4809__)
// Hardware ver. 2.1
#define HARDMPU_HW_NEW
// Default pin values/pullups
#define DEFAULT_PORTA 0b01110010    // TRIG, SPR4, RX0, TX0, IA1, WAIT, IA0, CLK
#define DEFAULT_DDRA  0b00001110
#define DEFAULT_PORTC 0b00111111    // N/A, N/A, /IRD, /IWR, USR3..0
#define DEFAULT_DDRC  0b00110000
#define DEFAULT_PORTD 0b11111111    // Internal Data Bus
#define DEFAULT_DDRD  0b00000000
#define DEFAULT_PORTE 0b00000011    // N/A, N/A, N/A, N/A, LED1, LED2, SPR2, SPR1
#define DEFAULT_DDRE  0b00001100
#define DEFAULT_PORTF 0b00111111    // N/A, /RST, SPR3, NC, NC, /INT, RX1, TX1
#define DEFAULT_DDRF  0b00000000
// gpio definitions
#define PORT_DATA   PORTD
#define DDR_DATA    DDRD
#define PINS_DATA   PIND
#define PORT_USER   PORTC
#define DDR_USER    DDRC
#define PINS_USER   PINC
#define PIN_USR0    (1<<PORTC0)
#define PIN_USR1    (1<<PORTC1)
#define PIN_USR2    (1<<PORTC2)
#define PIN_USR3    (1<<PORTC3)
#define PORT_IRW    PORTC
#define PIN_IWR     (1<<PORTC4)
#define PIN_IRD     (1<<PORTC5)
#define PINS_FLAGS  PORTD
#define PIN_DSR		(1<<PORTD7)     // isa data out flip-flop
#define PIN_DRR		(1<<PORTD6)     // isa data in flip-flop
#define PIN_CRR		(1<<PORTD5)     // isa command in flip-flop;
#define PORT_LED    PORTE
#define PIN_LED1    (1<<PORTE3)
#define PIN_LED2    (1<<PORTE2)
#define PINS_INTEXT PINF
#define PIN_INTEXT  (1<<PORTF2)
#define PORT_ADDR   PORTA
#define PIN_IA0     (1<<PORTA1)
#define PIN_IA1     (1<<PORTA3)
// clock parameters
#define F_CPU		20000000UL
#define BAUD_UART0	39              // 39 = 31250 (MIDI standard)
#define BAUD_UART1	39              // 10 = 115200-ish

#else
// Hardware ver. 1 or 2.0
#define HARDMPU_HW_OLD
// Default pin values/pullups
#define DEFAULT_PORTA 0b11111111    // Internal Data Bus
#define DEFAULT_DDRA  0b00000000
#define DEFAULT_PORTB 0b11111000	// ISP, ISP, ISP, /IDR, /IDW, CRR, DRR, DSR
#define DEFAULT_DDRB  0b00011000
#define DEFAULT_PORTC 0b11111111	// /TEST, LED, JTAG, JTAG, JTAG, JTAG, NC, NC
#define DEFAULT_DDRC  0b00000000
#define DEFAULT_PORTD 0b11111111	// USR3..0, TX1, RX1, TX0, RX0
#define DEFAULT_DDRD  0b00000000
// gpio definitions
#define PORT_DATA   PORTA
#define DDR_DATA    DDRA
#define PINS_DATA   PINA
#define PORT_IRW    PORTB
#define PIN_IDW		(1<<PORTB3)     // isa output latch CLK
#define PIN_IDR		(1<<PORTB4)     // isa input latch OC
#define PINS_FLAGS  PINB
#define PIN_DSR		(1<<PORTB0)     // isa data out flip-flop
#define PIN_DRR		(1<<PORTB1)     // isa data in flip-flop
#define PIN_CRR		(1<<PORTB2)     // isa command in flip-flop;
#define PORT_TEST   PORTC
#define DDR_TEST    DDRC
#define PINS_TEST   PINC
#define PIN_LED		(1<<PORTC6)     // test LED
#define PIN_TEST	(1<<PORTC7)     // test mode indicator
#define PORT_USER   PORTD
#define DDR_USER    DDRD
#define PINS_USER   PIND
#define PIN_USR0    (1<<PORTD4)
#define PIN_USR1    (1<<PORTD5)
#define PIN_USR2    (1<<PORTD6)
#define PIN_USR3    (1<<PORTD7)
// clock parameters
#define F_CPU		20000000UL
#define BAUD_UART0	39              // 39 = 31250 (MIDI standard)
#define BAUD_UART1	39              // 10 = 115200-ish

#endif
#endif
