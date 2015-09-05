#ifndef __CONFIG
#define __CONFIG

// pins
#define PIN_IDW		(1<<PORTB3)	// isa output latch CLK
#define PIN_IDR		(1<<PORTB4)	// isa input latch OC
#define PIN_DSR		(1<<PORTB0)	// isa data out flip-flop
#define PIN_DRR		(1<<PORTB1)	// isa data in flip-flop
#define PIN_CRR		(1<<PORTB2)	// isa command in flip-flop;

// parameters
#define F_CPU		20000000UL
#define BUFFER_SIZE	1024
#define BAUD_MIDI	39			// 31250
#define BAUD_UART	10			// 115200-ish

#endif