/*
 * HardMPU.c
 *
 * Created: 8/21/2015 2:47:37 PM
 *  Author: alex
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
//void send_midi_byte();

int main(void)
{
	// init GPIO
	//PORTA = 0b11111111;	// pullups enabled
	PORTB = 0b11111000;	// bits 0-2 are driven externally
	DDRB  = 0b00011000;	// data read and data write latches
	PORTC = 0b11111111;	// pullups enabled
	PORTD = 0b11111111;	// pullups enabled
	
	// init UART
	UCSR0B = (1<<TXEN0);//|(1<<RXEN0);
	UBRR0  = BAUD_MIDI;
	
	// init timer
	TCCR1B |= (1<<WGM12);		// timer1 ctc mode
	TIMSK1 |= (1<<OCIE1A);		// enable ctc interrupt
	sei();						// enable global interrupts
	OCR1A   = F_CPU / RTCFREQ;	// ctc value
	TCCR1B |= (1<<CS10);		// start timer with no prescaler
	
	// init emulator
	MPU401_Init(false, false);
	
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
		send_midi_byte();
		/* if (UCSR0A & (1<<RXC0)) {	// midi uart rx buffer is full
			process_midi_byte();
		} */
    }
}

void send_isa_byte(unsigned char byte) {
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
	return temp;					// report back with the results
	sei();							// re-enable interrupts
}

//void send_midi_byte() {
	//if (midi_out_buff.head == midi_out_buff.tail) return;		// nothing to send
	//UDR0 = midi_out_buff.buffer[midi_out_buff.tail];			// send the next byte
	//midi_out_buff.tail = (unsigned int)(midi_out_buff.tail + 1) % BUFFER_SIZE;	// increment tail, wrap to 0 if we're at the end
//}