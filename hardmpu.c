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
uint8_t active_uart = 0;

// function definitions
void SetPinCtrl(const PORT_t* port, const uint8_t* arr);

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

unsigned char recv_isa_byte()
{
	unsigned char temp;
	cli();							// disable interrupts
    
#if defined (HARDMPU_HW_OLD)
	PORT_IRW &= ~PIN_IDR;			// lower IDR
	__builtin_avr_delay_cycles(3);	// wait for i/o to settle
	temp = PINS_DATA;				// capture what we find there
	POR_IRW |= PIN_IDR;				// raise IDR
#elif defined (HARDMPU_HW_NEW)
    PORT_ADDR.OUTCLR = PIN_IA0;     // select data register
    __builtin_avr_delay_cycles(3);	// wait for logic to stabilize
    temp = PORT_DATA.IN;            // capture what we find there
    PORT_ADDR.OUTSET = PIN_IA0;     // go back to flag register (idle state)
#endif

	sei();							// re-enable interrupts
	return temp;					// report back with the results
}

unsigned char GetFlags()
{
#if defined (HARDMPU_HW_OLD)
    uint8_t flags = PINS_FLAGS;
#elif defined (HARDMPU_HW_NEW)
    uint8_t flags = PORT_FLAGS.IN;
#endif
    return flags;
}

void AVR_Init()
{
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
	UBRR0  = BAUD_UART0;
	UBRR1  = BAUD_UART1;
	
	// init timer
	TCCR1B |= (1<<WGM12)|(1<<CS10);	// timer1 ctc mode, no prescaler
	TIMSK1 |= (1<<OCIE1A);			// enable ctc interrupt
	OCR1A   = F_CPU / RTCFREQ - 1;	// ctc value
    
#elif defined (HARDMPU_HW_NEW)
    // Init Clock
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, 3);  // Select external clock, no CLKOUT
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0);  // No prescaler    
    
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
    PORTMUX.USARTROUTEA = 0b11001101;   // UART0 on PA4-5
    UART0.BAUD = BAUD_UART0;
    UART0.CTRLC = 3;            // Async, 8N1
    UART0.CTRLB = 0b11000000;   // Enable RX, TX, Normal mode
    UART1.BAUD = BAUD_UART1;
    UART1.CTRLC = 3;
    UART1.CTRLB = 0b11000000;
    
    // Set default active UART from INTEXT pin
    active_uart = !(PORT_INTEXT.IN & PIN_INTEXT);
    
    // Init Timers
    TCA0.SINGLE.PER = F_CPU / RTCFREQ - 1;
    TCA0.SINGLE.CTRLA = 1;
    TCA0.SINGLE.INTCTRL = 1;
#endif
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
#endif
    return;
}

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

void SetActiveUart(uint8_t port)
{
    if (port > (NUM_UARTS - 1)) port = (NUM_UARTS - 1); 
    active_uart = port;
}

/* Send a byte out into the world from the UART */
void output_to_uart(const uint8_t val)
{
#if defined (HARDMPU_HW_OLD)
	if (active_uart == 0)
	{
		UDR0 = val;
	}
	else
	{
		UDR1 = val;
	}
#elif defined (HARDMPU_HW_NEW)
    if (active_uart == 0)
    {
        UART0.TXDATAL = val;
    }
    else
    {
        UART1.TXDATAL = val;
    }
#endif
}

/* Wait for UART TX buffer to be empty */
void wait_for_uart()
{
#if defined (HARDMPU_HW_OLD)
	if (active_uart == 0)
	{
		loop_until_bit_is_set(UCSR0A, UDRE0);
	}
	else
	{
		loop_until_bit_is_set(UCSR1A, UDRE1);
	}
#elif defined (HARDMPU_HW_NEW)
    if (active_uart == 0)
    {
        loop_until_bit_is_set(UART0.STATUS, DREIF);
    }
    else
    {
        loop_until_bit_is_set(UART1.STATUS, DREIF);
    }
#endif
}

/* Check UART TX status, returns 0 for ready */
uint8_t uart_tx_status()
{
#if defined (HARDMPU_HW_OLD)
	if (active_uart == 0)
	{
		return bit_is_clear(UCSR0A, UDRE0);
	}
	else
	{
		return bit_is_clear(UCSR1A, UDRE1);
	}
#elif defined (HARDMPU_HW_NEW)
    if (active_uart == 0)
    {
        return bit_is_clear(UART0.STATUS, DREIF);
    }
    else
    {
        return bit_is_clear(UART1.STATUS, DREIF);
    }
#endif
}

void Sysex_Delay()
{
    // TODO: Wait State based delay on new hw
    _delay_us(320);
}

/* Periodic timer interrupt */
#if defined (HARDMPU_HW_OLD)
ISR(TIMER1_OVF_vect)
{
#elif defined (HARDMPU_HW_NEW)
ISR(TCA0_OVF_vect)
{
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
#endif
    PIC_Update();
}
