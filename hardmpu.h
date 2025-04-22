#ifndef HARDMPU_H
#define HARDMPU_H

#define FLAG_DSR (1<<0)
#define FLAG_DRR (1<<1)
#define FLAG_CRR (1<<2)
#define UART_EXT 0
#define UART_INT 1
#define UART_USR 2
#define UART_AUX 3
#define MAX_UART UART_AUX
// other flag bits are undefined and should be ignored

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
void TestMode();
unsigned char GetFlags();
void SetMidiUart(unsigned char);
unsigned char GetDefaultMidiPort();
void Sysex_Tarpit();
unsigned char MPU_Revision();

#endif