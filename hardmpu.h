#ifndef HARDMPU_H
#define HARDMPU_H

// Flag bits
#define FLAG_DSR (1<<0)     // 0 = Waiting for host to read data
#define FLAG_DRR (1<<1)     // 1 = Data ready from host
#define FLAG_CRR (1<<2)     // 1 = Command ready from host
// other flag bits are undefined and should be ignored

// (Logical) UART definitions
// See config.h for physical UART definitions
#define UART_EXT 0          // External MIDI port (DE9)
#define UART_INT 1          // Internal MIDI port (wavetable)
#define UART_USR 2          // User port UART
#define UART_AUX 3          // Second user port UART (shared with USR0/USR1)
#define MAX_UART UART_AUX   // Highest number UART (for parameter validation)

// function prototypes
void send_isa_byte(const unsigned char);
unsigned char recv_isa_byte();
void recv_ctl_byte();
void output_midi(const unsigned char);
void output_to_uart(const unsigned char, const unsigned char);
void wait_for_midi_uart();
void wait_for_uart(const unsigned char);
unsigned char midi_tx_status();
unsigned char uart_tx_status(const unsigned char);
void AVR_Init();
unsigned char GetFlags();
void SetMidiUart(unsigned char);
unsigned char GetDefaultMidiPort();
void Sysex_Tarpit();
unsigned char MPU_Revision();

#endif