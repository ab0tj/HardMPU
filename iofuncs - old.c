#include <avr/builtins.h>
#include <avr/io.h>
#include "config.h"

typedef struct ring_buffer {
	unsigned char buffer[BUFFER_SIZE];
	unsigned int head;
	unsigned int tail;
} ring_buffer;

ring_buffer data_out_buff = { {0}, 0, 0 };
ring_buffer midi_out_buff = { {0}, 0, 0 };

void send_isa_byte() {
	if (data_out_buff.head == data_out_buff.tail) return;	// nothing to send
	DDRA = 0xff;											// set porta to output
	PORTA = data_out_buff.buffer[data_out_buff.tail];		// output byte on porta
	PORTB &= ~PIN_IDW;										// lower IDW
	data_out_buff.tail = (unsigned int)(data_out_buff.tail + 1) % BUFFER_SIZE;	// increment tail, wrap to 0 if we're at the end
	PORTB |= PIN_IDW;			// raise IDW
	DDRA = 0;					// set the port back to input
}

unsigned char recv_isa_byte() {
	PORTB &= ~PIN_IDR;				// lower IDR
	__builtin_avr_delay_cycles(6);	// wait for i/o to settle
	unsigned char temp = PINA;		// capture what we find there
	PORTB |= PIN_IDR;				// raise IDR
	return temp;					// report back with the results
}

void queue_isa_byte(const unsigned char byte) {
	unsigned int next = (unsigned int)(data_out_buff.head + 1) % BUFFER_SIZE;
	if (next != data_out_buff.tail) {
		data_out_buff.buffer[data_out_buff.head] = byte;
		data_out_buff.head = next;
	}
}

void send_midi_byte() {
	if (midi_out_buff.head == midi_out_buff.tail) return;		// nothing to send
	UDR0 = midi_out_buff.buffer[midi_out_buff.tail];			// send the next byte
	midi_out_buff.tail = (unsigned int)(midi_out_buff.tail + 1) % BUFFER_SIZE;	// increment tail, wrap to 0 if we're at the end
}

void queue_midi_byte(const unsigned char byte) {
	unsigned int next = (unsigned int)(midi_out_buff.head + 1) % BUFFER_SIZE;
	if (next != midi_out_buff.tail) {
		midi_out_buff.buffer[midi_out_buff.head] = byte;
		midi_out_buff.head = next;
	}
}

void queue_midi_uart_byte() {
	unsigned int next = (unsigned int)(midi_out_buff.head + 1) % BUFFER_SIZE;
	if (next != midi_out_buff.tail) {	// don't read the byte in the isa latch if we're going to cause a buffer overrun
		midi_out_buff.buffer[midi_out_buff.head] = recv_isa_byte();
		midi_out_buff.head = next;
	}
}

void clear_buffers() {
	data_out_buff.head = data_out_buff.tail = 0;
	midi_out_buff.head = midi_out_buff.tail = 0;
}