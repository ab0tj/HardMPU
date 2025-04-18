#ifndef HARDMPU_H
#define HARDMPU_H

#define FLAG_DSR (1<<0)
#define FLAG_DRR (1<<1)
#define FLAG_CRR (1<<2)
// other flag bits are undefined and should be ignored

// function prototypes
void send_isa_byte(const unsigned char);
unsigned char recv_isa_byte();
void recv_ctl_byte();
void output_to_uart(const unsigned char);
void wait_for_uart();
unsigned char uart_tx_status();
void AVR_Init();
void TestMode();
unsigned char GetFlags();
void SetActiveUart(unsigned char);
void Sysex_Delay();

#endif