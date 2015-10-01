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

/*
 * --------------------------------------------
 * HardMPU by ab0tj - Hardware MPU-401 Emulator
 * --------------------------------------------
 * 
 * Based on SoftMPU by bjt which was
 * based on MPU401 code from DOSBox
 *
 */

#include <avr/io.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include "config.h"
#include "EXPORT.H"

// function prototypes
void send_isa_byte(unsigned char byte);
unsigned char recv_isa_byte();
void recv_ctl_byte();

int main(void)
{
	// init GPIO
	PORTB = 0b11111000;	// bits 0-2 are driven externally
	DDRB  = 0b00011000;	// data read and data write latches
	PORTC = 0b11111111;	// pullups enabled
	PORTD = 0b11111111;	// pullups enabled
	
	// init UART
	UCSR0B = (1<<TXEN0);//|(1<<RXEN0);
	UBRR0  = BAUD_MIDI;
	
	// init timer
	TCCR1B |= (1<<WGM12)|(1<<CS10);	// timer1 ctc mode, no prescaler
	TIMSK1 |= (1<<OCIE1A);			// enable ctc interrupt
	OCR1A   = F_CPU / RTCFREQ - 1;	// ctc value
	sei();							// enable global interrupts
	
	// init emulator
	MPU401_Init();
	
    while(1)	// main loop
    {
		// do isa i/o
		if (PINB & PIN_CRR) {		// isa control input latch is full
			MPU401_WriteCommand(recv_isa_byte());
		}
		if (PINB & PIN_DRR) {		// isa data input latch is full
			MPU401_WriteData(recv_isa_byte());
		}
		if (QueueUsed() && (~PINB & PIN_DSR)) {
			send_isa_byte(MPU401_ReadData());		// send data if there's any in the buffer
		}
		
		// do midi i/o
		send_midi_byte();				// see if we need to send a byte		
		/* if (UCSR0A & (1<<RXC0)) {	// midi uart rx buffer is full
			process_midi_byte();
		} */
    }
}

void send_isa_byte(Bit8u byte) {
	cli();							// disable interrupts
	DDRA = 0xff;					// set porta to output
	PORTA = byte;					// output byte on porta
	PORTB &= ~PIN_IDW;				// lower IDW
	__builtin_avr_delay_cycles(3);	// wait for logic to stabilize
	PORTB |= PIN_IDW;				// raise IDW
	DDRA = 0;						// set the port back to input
	sei();							// re-enable interrupts
}

unsigned char recv_isa_byte() {
	unsigned char temp;
	cli();							// disable interrupts
	PORTB &= ~PIN_IDR;				// lower IDR
	__builtin_avr_delay_cycles(3);	// wait for i/o to settle
	temp = PINA;					// capture what we find there
	PORTB |= PIN_IDR;				// raise IDR
	sei();							// re-enable interrupts
	return temp;					// report back with the results
}