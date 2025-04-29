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

#include <avr/pgmspace.h>

#if defined (__AVR_ATmega4809__)
// Hardware ver. 2.1
#define HARDMPU_HW_NEW
#define MPU_REVISION 2
// Default port output values
#define DEFAULT_OUTA 0b00000000     // TRIG, SPR4, RX0, TX0, IA1, WAIT, IA0, CLK
#define DEFAULT_DIRA 0b00011110
#define DEFAULT_OUTC 0b00000000     // N/A, N/A, /IRD, /IWR, USR3..0
#define DEFAULT_DIRC 0b00110000
#define DEFAULT_OUTD 0b11111111     // Internal Data Bus
#define DEFAULT_DIRD 0b00000000
#define DEFAULT_OUTE 0b00000000     // N/A, N/A, N/A, N/A, LED1, LED2, SPR2, SPR1
#define DEFAULT_DIRE 0b00001100
#define DEFAULT_OUTF 0b00000001     // N/A, /RST, SPR3, /TEST, NC, /INT, RX1, TX1
#define DEFAULT_DIRF 0b00000000
// port definitions
#define PORT_DATA   PORTD           // Internal Data Bus
#define PORT_USER   PORTC           // User Port
#define PORT_IRW    PORTC           // Data Strobes
#define PORT_FLAGS  PORTD           // CPLD Status Flags
#define PORT_LED    PORTE           // LED Control
#define PORT_INTEXT PORTF           // Internal/External MIDI Jumper
#define PORT_ADDR   PORTA           // Internal Address
#define PORT_TEST   PORTF           // Test pin
// pin definitions
#define PIN_USR0    (1<<0)          // User port pins
#define PIN_USR1    (1<<1)
#define PIN_USR2    (1<<2)
#define PIN_USR3    (1<<3)
#define PIN_IWR     (1<<4)          // Write to CPLD
#define PIN_IRD     (1<<5)          // Read from CPLD
#define PIN_DSR		(1<<0)          // ISA data out ready
#define PIN_DRR		(1<<1)          // ISA data in ready
#define PIN_CRR		(1<<2)          // ISA command in ready
#define PIN_LED1    (1<<3)          // MIDI1 LED
#define PIN_LED2    (1<<2)          // MIDI2 LED
#define PIN_INTEXT  (1<<2)          // INT/EXT Jumper
#define PIN_IA0     (1<<1)          // Internal Address bit 0
#define PIN_IA1     (1<<3)          // Internal Address bit 1
#define PIN_TEST    (1<<4)          // Test pin
// pin control
#define PC_INVEN        (1<<7)
#define PC_PULLUPEN     (1<<3)
#define PC_INTDISABLE   0
#define PC_BOTHEDGES    1
#define PC_RISING       2
#define PC_FALLING      3
#define PC_INPUT_DIS    4
#define PC_LEVEL        5
const unsigned char PORTA_CTRL[8] PROGMEM =
{
    0,
    PC_INPUT_DIS,
    PC_INPUT_DIS,
    PC_INPUT_DIS,
    PC_INPUT_DIS,
    PC_PULLUPEN,
    (PC_PULLUPEN|PC_INPUT_DIS),
    0
};
const unsigned char PORTC_CTRL[8] PROGMEM =
{
    PC_PULLUPEN,
    PC_PULLUPEN,
    PC_PULLUPEN,
    PC_PULLUPEN,
    (PC_INVEN|PC_INPUT_DIS),
    (PC_INVEN|PC_INPUT_DIS),
    PC_INPUT_DIS,
    PC_INPUT_DIS
};
const unsigned char PORTD_CTRL[8] PROGMEM =
{
    PC_PULLUPEN,
    PC_PULLUPEN,
    PC_PULLUPEN,
    PC_PULLUPEN,
    PC_PULLUPEN,
    PC_PULLUPEN,
    PC_PULLUPEN,
    PC_PULLUPEN
};
const unsigned char PORTE_CTRL[8] PROGMEM =
{
    (PC_PULLUPEN|PC_INPUT_DIS),
    (PC_PULLUPEN|PC_INPUT_DIS),
    PC_INPUT_DIS,
    PC_INPUT_DIS,
    PC_INPUT_DIS,
    PC_INPUT_DIS,
    PC_INPUT_DIS,
    PC_INPUT_DIS
};
const unsigned char PORTF_CTRL[8] PROGMEM =
{
    PC_INPUT_DIS,
    PC_PULLUPEN,
    PC_PULLUPEN,
    (PC_PULLUPEN|PC_INPUT_DIS),
    PC_PULLUPEN,
    (PC_PULLUPEN|PC_INPUT_DIS),
    0,
    PC_INPUT_DIS
};
// clock parameters
#define F_CPU		20000000UL          // 20MHz
#define LED_TIMEOUT 40                  // 40 = 10ms
#define WAIT_TIME   300                 // 300 = 15us (ISA spec says 15.6us max)
#define SYSEX_DELAY 6400                // 6400 = 320us
// CPLD register addresses
#define ADDR_DATA 0
#define ADDR_CTRL 1
// UART definitions
#define PHYS_UART_EXT   USART0          // J1 MIDI Port
#define PHYS_UART_INT   USART2          // Wavetable header
#define PHYS_UART_USR   PHYS_UART_INT   // User port pins 1,3 (shared with INT)
#define PHYS_UART_AUX   USART1          // User port pins 9,10
//#define ENABLE_AUX_UART               // Uncomment to use UART on USR0,1 instead of GPIO
#define BAUD_EXT        2560            // 2560 = 31250 (MIDI standard)
#define BAUD_INT        2560            //   8333 = 9600-ish
#define BAUD_USR        BAUD_INT        //   694  = 115200-ish
#define BAUD_AUX        694             //   (F_CPU*64)/(BAUD*16)
#define DREIF           5

#else
// Hardware ver. 1.1 or 2.0
#define HARDMPU_HW_OLD
#define MPU_REVISION 1
// Default port values/pullups
#define DEFAULT_PORTA 0b11111111    // Internal Data Bus
#define DEFAULT_DDRA  0b00000000
#define DEFAULT_PORTB 0b11111000	// ISP, ISP, ISP, /IDR, /IDW, CRR, DRR, DSR
#define DEFAULT_DDRB  0b00011000
#define DEFAULT_PORTC 0b11111111	// /TEST, LED, JTAG, JTAG, JTAG, JTAG, NC, NC
#define DEFAULT_DDRC  0b00000000
#define DEFAULT_PORTD 0b11111111	// USR3..0, TX1, RX1, TX0, RX0
#define DEFAULT_DDRD  0b00000000
// port definitions
#define PORT_DATA   PORTA
#define DDR_DATA    DDRA
#define PINS_DATA   PINA
#define PORT_IRW    PORTB
#define PORT_TEST   PORTC
#define DDR_TEST    DDRC
#define PINS_TEST   PINC
#define PORT_USER   PORTD
#define DDR_USER    DDRD
#define PINS_USER   PIND
#define PINS_FLAGS  PINB
// pin definitions
#define PIN_IDW		(1<<3)          // isa output latch CLK
#define PIN_IDR		(1<<4)          // isa input latch OC
#define PIN_DSR		(1<<0)          // isa data out flip-flop
#define PIN_DRR		(1<<1)          // isa data in flip-flop
#define PIN_CRR		(1<<2)          // isa command in flip-flop;
#define PIN_LED		(1<<6)          // test LED
#define PIN_TEST	(1<<7)          // test mode indicator
#define PIN_USR0    (1<<4)
#define PIN_USR1    (1<<5)
#define PIN_USR2    (1<<6)
#define PIN_USR3    (1<<7)
// clock parameters
#define F_CPU		20000000UL
// UART definitions
#define BAUD_EXT        39          // 39 = 31250 (MIDI standard)
#define BAUD_INT        39          // 10 = 115200-ish
#endif

#endif
