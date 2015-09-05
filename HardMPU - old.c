/*
 * HardMPU.c
 *
 * Created: 8/21/2015 2:47:37 PM
 *  Author: alex
 */ 

#include <avr/io.h>
#include "config.h"
#include "iofuncs.h"

// function prototypes
static void process_data_byte();
static void process_ctrl_byte();
static void process_midi_byte();
static void system_reset();

char uart_mode = 0;

int main(void)
{
	// init GPIO
	PORTA = 0b11111111;	// pullups enabled
	PORTB = 0b11111000;	// bits 0-2 are driven externally
	DDRB  = 0b00011000;	// data read and data write latches
	PORTC = 0b11111111;	// pullups enabled
	PORTD = 0b11111111;	// pullups enabled
	
	// init UART
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	UBRR0H = (unsigned char)(BAUD_MIDI>>8);
	UBRR0L = (unsigned char)BAUD_MIDI;
	
    while(1)	// main loop
    {
        // first do isa i/o
		if (PINB & PIN_DRR) {		// isa data input latch is full
			process_data_byte();
		}
		if (PINB & PIN_CRR) {		// isa control input latch is full
			process_ctrl_byte();
		}
		if (~PINB & PIN_DSR) {		// isa data output latch is empty
			send_isa_byte();		// send data if there's any in the buffer
		}
		
		// next do midi i/o
		if (UCSR0A & (1<<UDRE0)) {	// midi uart tx buffer is empty
			send_midi_byte();
		}
		if (UCSR0A & (1<<RXC0)) {	// midi uart rx buffer is full
			process_midi_byte();
		}
    }
}

void process_data_byte() {
	if (uart_mode) {
		queue_midi_uart_byte();
	} else {
		recv_isa_byte();	// drop the byte for now
		// do intelligent stuffs
	}
}

void process_ctrl_byte() {
	switch (recv_isa_byte()) {
		case 0x3f:	// switch to uart mode
			uart_mode = 1;
			clear_buffers();
			break;
		case 0xff:	// reset
			system_reset();
			queue_isa_byte(0xfe);	// ack
			break;
	}
}

void process_midi_byte() {
	if (uart_mode) {
		queue_isa_byte(UDR0);
	} else {
		// do intelligent stuffs
	}
}

void system_reset() {	// reset to power-on state
	uart_mode = 0;
	clear_buffers();
}