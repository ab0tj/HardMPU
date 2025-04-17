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

/*
 * --------------------------------------------
 * HardMPU by ab0tj - Hardware MPU-401 Emulator
 * --------------------------------------------
 * 
 * Based on SoftMPU by bjt which was
 * based on MPU401 code from DOSBox
 *
 */

#include "config.h"
#include "export.h"
#include <avr/io.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#if defined (HARDMPU_HW_NEW)
FUSES = {
	.WDTCFG = 0x00, // WDTCFG {PERIOD=OFF, WINDOW=OFF}
	.BODCFG = 0xE5, // BODCFG {SLEEP=ENABLED, ACTIVE=ENABLED, SAMPFREQ=1KHZ, LVL=BODLEVEL7}
	.OSCCFG = 0x02, // OSCCFG {FREQSEL=20MHZ, OSCLOCK=CLEAR}
	.SYSCFG0 = 0xFE, // SYSCFG0 {EESAVE=CLEAR, RSTPINCFG=RST, CRCSRC=NOCRC}
	.SYSCFG1 = 0x00, // SYSCFG1 {SUT=0MS}
	.APPEND = 0x00, // APPEND {APPEND=User range:  0x0 - 0xFF}
	.BOOTEND = 0x00, // BOOTEND {BOOTEND=User range:  0x0 - 0xFF}
};

LOCKBITS = 0xC5; // {LB=NOLOCK}
#endif

// function prototypes
void send_isa_byte(unsigned char byte);
unsigned char recv_isa_byte();
void recv_ctl_byte();
void AVR_Init();

int main(void)
{
    // init hardware
    AVR_Init();
	
	// init emulator
	MPU401_Init();
	
	// enable interrupts
	sei();
	
    for(;;)	// main loop
    {
		// do isa i/o
		if (QueueUsed() && (~PINB & PIN_DSR)) {
			send_isa_byte(MPU401_ReadData());		// send data if there's any in the buffer
		}
		if (PINB & PIN_CRR) {		// isa control input latch is full
			MPU401_WriteCommand(recv_isa_byte());
		}
		if (PINB & PIN_DRR) {		// isa data input latch is full
			MPU401_WriteData(recv_isa_byte());
		}
		// do midi i/o
		send_midi_byte();				// see if we need to send a byte	
		/* if (UCSR0A & (1<<RXC0)) {	// midi uart rx buffer is full
			process_midi_byte();
		} */
    }
}

void send_isa_byte(Bit8u byte)
{
	cli();							// disable interrupts
	DDRA = 0xff;					// set porta to output
	PORTA = byte;					// output byte on porta
	PORTB &= ~PIN_IDW;				// lower IDW
	__builtin_avr_delay_cycles(3);	// wait for logic to stabilize
	PORTB |= PIN_IDW;				// raise IDW
	DDRA = 0;						// set the port back to input
	sei();							// re-enable interrupts
}

unsigned char recv_isa_byte()
{
	unsigned char temp;
	cli();							// disable interrupts
	PORTB &= ~PIN_IDR;				// lower IDR
	__builtin_avr_delay_cycles(3);	// wait for i/o to settle
	temp = PINA;					// capture what we find there
	PORTB |= PIN_IDR;				// raise IDR
	sei();							// re-enable interrupts
	return temp;					// report back with the results
}

void AVR_Init()
{
    // Init GPIO
#if defined (DEFAULT_PORTA)
    PORTA = DEFAULT_PORTA;
    DDRA = DEFAULT_DDRA;
#endif
#if defined (DEFAULT_PORTB)
    PORTB = DEFAULT_PORTB;
    DDRB = DEFAULT_DDRB;
#endif
#if defined (DEFAULT_PORTC)
    PORTC = DEFAULT_PORTC;
    DDRC = DEFAULT_DDRC;
#endif
#if defined (DEFAULT_PORTD)
    PORTD = DEFAULT_PORTD;
    DDRD = DEFAULT_DDRD;
#endif
#if defined (DEFAULT_PORTE)
    PORTE = DEFAULT_PORTE;
    DDRE = DEFAULT_DDRE;
#endif
#if defined (DEFAULT_PORTF)
    PORTF = DEFAULT_PORTF;
    DDRF = DEFAULT_DDRF;
#endif
	
#if defined (HARDMPU_HW_OLD)
	// are we in the programming jig?
	if (~PINS_TEST & PIN_TEST)
	{
		DDR_TEST |= PIN_LED;	// LED pin output
		
		for (;;)	// blink the LED
		{
			PORT_TEST &= ~PIN_LED;
			_delay_ms(125);
			PORT_TEST |= PIN_LED;
			_delay_ms(125);
		}
	}
#endif
	
	// init UART
	UCSR0B = (1<<TXEN0);//|(1<<RXEN0);
	UCSR1B = (1<<TXEN1);
	UBRR0  = BAUD_UART0;
	UBRR1  = BAUD_UART1;
	
	// init timer
	TCCR1B |= (1<<WGM12)|(1<<CS10);	// timer1 ctc mode, no prescaler
	TIMSK1 |= (1<<OCIE1A);			// enable ctc interrupt
	OCR1A   = F_CPU / RTCFREQ - 1;	// ctc value
}