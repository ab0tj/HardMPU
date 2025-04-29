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

/* hardmpu.c: This is where all the hardware interface things happen. */

#include "config.h"
#include "export.h"
#include "hardmpu.h"
#include <avr/io.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#if defined (HARDMPU_HW_OLD)
    // old hw fuses go here
#elif defined (HARDMPU_HW_NEW)
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
#else
#error "HardMPU hardware version not defined"
#endif

// running config variables
uint8_t midi_uart = UART_EXT;
volatile uint8_t led_timeout[2];

// function definitions
void SetPinCtrl(const PORT_t* port, const uint8_t* arr);
void TestMode();

// Send a byte to the host sytem
void send_isa_byte(const Bit8u byte)
{
	cli();							// disable interrupts
    
#if defined (HARDMPU_HW_OLD)
	DDR_DATA = 0xff;				// set porta to output
	PORT_DATA = byte;				// output byte on porta
	PORT_IRW &= ~PIN_IDW;   		// lower IDW
	__builtin_avr_delay_cycles(3);	// wait for logic to stabilize
	PORT_IRW |= PIN_IDW;			// raise IDW
	DDR_DATA = 0;					// set the port back to input
#elif defined (HARDMPU_HW_NEW)
    PORT_IRW.OUTCLR = PIN_IRD;      // stop CPLD from outputting
    PORT_ADDR.OUTCLR = PIN_IA0;     // select data register
    PORT_DATA.DIR = 0xff;           // set data port to output
    PORT_DATA.OUT = byte;           // set value on data port
    PORT_IRW.OUTSET = PIN_IWR;      // clock value into the CPLD
    PORT_IRW.OUTCLR = PIN_IWR;
    PORT_DATA.DIR = 0;              // set data port back to input
    PORT_ADDR.OUTSET = PIN_IA0;     // select flag register
    PORT_IRW.OUTSET = PIN_IRD;      // read flag register (idle state)
#endif
    
	sei();							// re-enable interrupts
}

// Get a byte from the host system
unsigned char recv_isa_byte()
{
	unsigned char val;
	cli();							// disable interrupts
    
#if defined (HARDMPU_HW_OLD)
	PORT_IRW &= ~PIN_IDR;			// lower IDR
	__builtin_avr_delay_cycles(3);	// wait for i/o to settle
	val = PINS_DATA;				// capture what we find there
	POR_IRW |= PIN_IDR;				// raise IDR
#elif defined (HARDMPU_HW_NEW)
    PORT_ADDR.OUTCLR = PIN_IA0;     // select data register
    __builtin_avr_delay_cycles(3);	// wait for logic to stabilize
    val = PORT_DATA.IN;             // capture what we find there
    PORT_ADDR.OUTSET = PIN_IA0;     // go back to flag register (idle state)
#endif

	sei();							// re-enable interrupts
	return val;                     // report back with the results
}

// Read the handshaking status flags
// See FLAG* defs for description
unsigned char GetFlags()
{
#if defined (HARDMPU_HW_OLD)
    return PINS_FLAGS;              // Read directly from GPIO port
#elif defined (HARDMPU_HW_NEW)
    return PORT_FLAGS.IN;           // Data bus has flags in idle state
#endif
}

// Initialize the hardware
void AVR_Init()
{
    cli();                          // Disable interrupts for now
    
#if defined (HARDMPU_HW_OLD)
    // Init GPIO
    PORTA = DEFAULT_PORTA;
    DDRA = DEFAULT_DDRA;
    PORTB = DEFAULT_PORTB;
    DDRB = DEFAULT_DDRB;
    PORTC = DEFAULT_PORTC;
    DDRC = DEFAULT_DDRC;
    PORTD = DEFAULT_PORTD;
    DDRD = DEFAULT_DDRD;
    
    // init UART
	UCSR0B = (1<<TXEN0);//|(1<<RXEN0);
	UCSR1B = (1<<TXEN1);
	UBRR0  = BAUD_EXT;
	UBRR1  = BAUD_INT;
	
	// init timer
	TCCR1B |= (1<<WGM12)|(1<<CS10);	// timer1 ctc mode, no prescaler
	TIMSK1 |= (1<<OCIE1A);			// enable ctc interrupt
	OCR1A   = F_CPU / RTCFREQ - 1;	// ctc value
    
    // Go to test mode if applicable
    TestMode();
    
#elif defined (HARDMPU_HW_NEW)
    // Init Clock
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, 3);         // Select external clock, no CLKOUT
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0);         // No prescaler
    loop_until_bit_is_clear(CLKCTRL.MCLKSTATUS, 0); // Wait for the switch to complete
    
    // Init GPIO
    SetPinCtrl(&PORTA, PORTA_CTRL);
    PORTA.OUT = DEFAULT_OUTA;
    PORTA.DIR = DEFAULT_DIRA;
    SetPinCtrl(&PORTC, PORTC_CTRL);
    PORTC.OUT = DEFAULT_OUTC;
    PORTC.DIR = DEFAULT_DIRC;
    SetPinCtrl(&PORTD, PORTD_CTRL);
    PORTD.OUT = DEFAULT_OUTD;
    PORTD.DIR = DEFAULT_DIRD;
    SetPinCtrl(&PORTE, PORTE_CTRL);
    PORTE.OUT = DEFAULT_OUTE;
    PORTE.DIR = DEFAULT_DIRE;
    SetPinCtrl(&PORTF, PORTF_CTRL);
    PORTF.OUT = DEFAULT_OUTF;
    PORTF.DIR = DEFAULT_DIRF;
    
    // Set idle state (reading CPLD flag register)
    PORT_IRW.OUTCLR = PIN_IWR;
    PORT_ADDR.OUTSET = PIN_IA0;
    PORT_IRW.OUTSET = PIN_IRD;
    
    // Init UARTs
    PORTMUX.USARTROUTEA = 0b11001101;           // UART0 on PA4-5
    PHYS_UART_EXT.BAUD = BAUD_EXT;
    PHYS_UART_EXT.CTRLC = 3;                    // Async, 8N1
    PHYS_UART_EXT.CTRLB = 0b11000000;           // Enable RX, TX, Normal mode
    PHYS_UART_INT.BAUD = BAUD_INT;
    PHYS_UART_INT.CTRLC = 3;
    PHYS_UART_INT.CTRLB = 0b11000000;
#if defined (ENABLE_AUX_UART)
    PHYS_UART_AUX.BAUD = BAUD_AUX;
    PHYS_UART_AUX.CTRLC = 3;
    PHYS_UART_AUX.CTRLB = 0b11000000;
#endif
    
    // Reset MIDI devices (and get the UARTs rolling)
    output_to_uart(UART_INT, 0xff);
    output_to_uart(UART_EXT, 0xff);
    
    // Init Timers
    // RTC Timer
    TCA0.SINGLE.PER = F_CPU / RTCFREQ - 1;
    TCA0.SINGLE.CTRLA = 1;                      // No prescaler, enable timer
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;    // Enable overflow interrupt
    // Wait State Generator
    TCB0.CCMP = WAIT_TIME;
    TCB0.CTRLB =  0b01010110;       // Async, output enable, single-shot mode
    EVSYS.CHANNEL0 = 0b01000111;    // Connect trigger input to event channel 0
    EVSYS.USERTCB0 = 1;             // Connect TCB0 to event channel 0
    TCB0.EVCTRL = 1;                // Enable capture event
    // SysEx Delay Timer
    TCB1.CCMP = SYSEX_DELAY;
    TCB1.CTRLB = 0b01000110;        // Async, no output, single-shot mode
    EVSYS.USERTCB1 = 2;             // Connect TCB1 to event channel 1
    TCB1.EVCTRL = 1;                // Enable capture event
    TCB1.CNT = SYSEX_DELAY;         // Prevent timer from starting immediately
    TCB1.INTCTRL = 1;               // Enable interrupt
    TCB1.CTRLA = 1;                 // Enable timer
    TCB1.INTFLAGS = 1;              // Clear interrupt flag
#endif
    
    // Set default active UART
    midi_uart = GetDefaultMidiPort();
}

void TestMode()
// currently does nothing on the new hardware version
{
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
#elif defined (HARDMPU_HW_NEW)
    if (~PORT_TEST.IN & PIN_TEST)
    {
        for (;;)
        {
            PORT_LED.OUTSET = PIN_LED1;
            _delay_ms(125);
            PORT_LED.OUTCLR = PIN_LED1;
            _delay_ms(125);
        }
    }
#endif
    return;
}

// Set pin control settings on a GPIO port
void SetPinCtrl(const PORT_t* port, const uint8_t* arr)
{
#if defined (HARDMPU_HW_NEW)
    uint8_t ctl;
    volatile uint8_t* pinctrl = (volatile uint8_t*)(&(port->PIN0CTRL));
    for (int i = 0; i < 8; i++)
    {
        ctl = pgm_read_byte(arr++);
        *pinctrl = ctl;
        pinctrl++;
    }
#endif
    return;
}

// Set the active UART for MIDI messages
void SetMidiUart(uint8_t port)
{
    if (port > MAX_UART) port = UART_EXT;
    midi_uart = port;
}

// Returns the (logical) UART selected as the default (by a jumper or otherwise)
uint8_t GetDefaultMidiPort()
{
#if defined (HARDMPU_HW_NEW)
    // Get default from jumper value
    if (PORT_INTEXT.IN & PIN_INTEXT)
    {
        return UART_EXT;
    }
    else
    {
        return UART_INT;
    }
#else
    // This hardware has no way to set a default (yet))
    return UART_EXT;
#endif
}

/* Send a byte to the active MIDI port */
void output_midi(const uint8_t val)
{
    output_to_uart(midi_uart, val);
#if defined (HARDMPU_HW_NEW)
    /* midi.c doesn't send bytes during SysEx delay, so
     * we can disable the wait state generator now. */
    TCB0.CTRLA = 0;
#endif
}

/* Send a byte out into the world from a UART */
void output_to_uart(const uint8_t uart, const uint8_t val)
{
#if defined (HARDMPU_HW_OLD)
	switch (uart)
    {
        case UART_EXT:
            UDR0 = val;
            break;
        case UART_INT:
        case UART_USR:
            UDR1 = val;
            break;
	}
#elif defined (HARDMPU_HW_NEW)
    switch (uart)
    {
        case UART_EXT:
            PHYS_UART_EXT.TXDATAL = val;
            PORT_LED.OUTSET = PIN_LED1;
            led_timeout[0] = LED_TIMEOUT;
            break;
        case UART_INT:
            PHYS_UART_INT.TXDATAL = val;
            PORT_LED.OUTSET = PIN_LED2;
            led_timeout[1] = LED_TIMEOUT;
            break;
        case UART_USR:
            PHYS_UART_USR.TXDATAL = val;
            break;
#if defined (ENABLE_AUX_UART)
        case UART_AUX:
            UART_AUX.TXDATAL = val;
#endif
            break;
    }
#endif
}

/* Wait for active MIDI UART TX buffer to be empty */
void wait_for_midi_uart()
{
    wait_for_uart(midi_uart);
}

/* Wait for UART TX buffer to be empty */
void wait_for_uart(const uint8_t uart)
{
#if defined (HARDMPU_HW_OLD)
	switch (uart)
	{
        case UART_EXT:
            loop_until_bit_is_set(UCSR0A, UDRE0);
            break;
        case UART_INT:
        case UART_USR:
            loop_until_bit_is_set(UCSR1A, UDRE1);
            break;
	}
#elif defined (HARDMPU_HW_NEW)
    switch (uart)
    {
        case UART_EXT:
            loop_until_bit_is_set(PHYS_UART_EXT.STATUS, DREIF);
            break;
        case UART_INT:
        case UART_USR:
            loop_until_bit_is_set(PHYS_UART_INT.STATUS, DREIF);
            break;
#if defined (ENABLE_AUX_UART)
        case UART_AUX:
            loop_until_bit_is_set(PHYS_UART_AUX.STATUS, DREIF);
            break;
#endif
    }
#endif
}

/* Check MIDI UART TX status, returns 0 for ready */
uint8_t midi_tx_status()
{
    return uart_tx_status(midi_uart);
}

/* Check UART TX status, returns 0 for ready */
uint8_t uart_tx_status(const uint8_t uart)
{
#if defined (HARDMPU_HW_OLD)
	switch (uart)
	{
        case UART_EXT:
            return bit_is_clear(UCSR0A, UDRE0);
        case UART_INT:
        case UART_USR:
            return bit_is_clear(UCSR1A, UDRE1);
        default:
            return 1;
	}
#elif defined (HARDMPU_HW_NEW)
    switch (uart)
    {
        case UART_EXT:
            return bit_is_clear(PHYS_UART_EXT.STATUS, DREIF);
        case UART_INT:
        case UART_USR:
            return bit_is_clear(PHYS_UART_INT.STATUS, DREIF);
#if defined (ENABLE_AUX_UART)
        case UART_AUX:
            return bit_is_clear(PHYS_UART_AUX.STATUS, DREIF);
#endif
        default:
            return 1;
    }
#endif
}

/* Attempt to slow down the host system to help avoid buffer overruns */
void Sysex_Tarpit()
{
#if defined (HARDMPU_HW_OLD)
    _delay_us(320);             // no wait state generator on v1 hardware
#elif defined (HARDMPU_HW_NEW)
    if (!(TCB0.CTRLA & 1))      // Wait state generator not enabled
    {
        TCB0.CNT = WAIT_TIME;   // Prevent timer from running immediately
        TCB0.CTRLA = 1;         // Enable wait state generator
        //EVSYS.STROBE = 2;     // Trigger the delay timer
    }
#endif
}

/* Return the hardware revision to be reported to the host system */
uint8_t MPU_Revision()
{
    return MPU_REVISION;
}

/* Periodic timer interrupt */
#if defined (HARDMPU_HW_OLD)
ISR(TIMER1_OVF_vect)
{
#elif defined (HARDMPU_HW_NEW)
ISR(TCA0_OVF_vect)
{
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;   // Clear interrupt flag
    
    // Check if it's time to shut off LEDs
    if (led_timeout[0] != 0)
    {
        led_timeout[0]--;
        if (led_timeout[0] == 0) PORT_LED.OUTCLR = PIN_LED1;
    }
    if (led_timeout[1] != 0)
    {
        led_timeout[1]--;
        if (led_timeout[1] == 0) PORT_LED.OUTCLR = PIN_LED2;
    }
#endif
    
    // Do MPU401 things
    PIC_Update();
}

/* Sysex Delay Timer Expired */
//#if defined (HARDMPU_HW_NEW)
//ISR(TCB1_INT_vect)
//{
//    TCB1.INTFLAGS = 1;      // Clear interrupt flag
//    TCB0.CTRLA = 0;         // Disable wait-state generator
//}
//#endif