#include <avr/interrupt.h>
#include "hardmpu.h"
#include "export.h"

int main(void)
{
    uint8_t flags;
    
    // init hardware
    AVR_Init();
    
    // check if we need to be in test mode
    TestMode();
	
	// init emulator
	MPU401_Init();
	
	// enable interrupts
	sei();
	
    for(;;)	// main loop
    {
		// do isa i/o
        flags = GetFlags();
		if (QueueUsed() && (flags & FLAG_DSR)) {
			send_isa_byte(MPU401_ReadData());		// send data if there's any in the buffer
		}
		if (flags & FLAG_CRR) {		// isa control input latch is full
			MPU401_WriteCommand(recv_isa_byte());
		}
		if (flags & FLAG_DRR) {		// isa data input latch is full
			MPU401_WriteData(recv_isa_byte());
		}
        
		// do midi i/o
		send_midi_byte();				// see if we need to send a byte	
		/* if (UCSR0A & (1<<RXC0)) {	// midi uart rx buffer is full
			process_midi_byte();
		} */
    }
}