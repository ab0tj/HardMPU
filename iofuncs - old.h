#ifndef __IOFUNCS_H
#define __IOFUNCS_H

// functions
void send_isa_byte();
unsigned char recv_isa_byte();
void queue_isa_byte(const unsigned char);
void send_midi_byte();
void queue_midi_byte(const unsigned char);
void queue_midi_uart_byte();
void clear_buffers();

#endif