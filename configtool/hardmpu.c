#include <conio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <time.h>

#define CFG_DELAY   0x80
#define CFG_FAKEOFF 0x40
#define CFG_VERFIX  0x20
#define CFG_MIDIPT  0x10
#define MPU_DSR     0x80
#define MPU_DRR     0x40

void show_help(char*);
void send_byte(int, int, unsigned char);
void wait_for_ack(int);
unsigned char get_byte(int);

int main(int argc, char *argv[]) {
    int c;
    int port = 0x330;
    unsigned char config = 0;
    int setconfig = 1;

    cprintf("\r\n-- AB0TJ HardMPU Configuration Utility --\r\n\r\n");

    while((c = getopt(argc,argv,":p:dfghsi")) != -1) {
        switch(c) {
            case 'p':
                port = strtol(optarg,NULL,16);
                break;
            case 'd':
                config |= CFG_DELAY;
                break;
            case 'f':
                config |= CFG_FAKEOFF;
                break;
            case 'g':
                config |= CFG_VERFIX;
                break;
            case 's':
                setconfig = 0;
                break;
            case 'i':
                config |= CFG_MIDIPT;
                break;
            case 'h':
                show_help(basename(argv[0]));
                return 0;
            case '?':
                cprintf("Unknown command line option: -%c\r\n\r\n", optopt);
                show_help(basename(argv[0]));
                return 1;
            case ':':
                cprintf("Command line option missing argument: -%c\r\n\r\n", optopt);
                show_help(basename(argv[0]));
                return 1;
            default:
                cprintf("Internal error when processing command line options: %c\r\n", optopt);
                return 1;
        }
    }

    cprintf("Using port 0h%x.\r\n", port);

    send_byte(port+1, port+1, 0xff);  // reset the mpu
    wait_for_ack(port);

    if (setconfig) {
        send_byte(port+1, port+1, 0xfe);
        wait_for_ack(port);
        send_byte(port, port+1, config);
        wait_for_ack(port);
    } else {
        send_byte(port+1, port+1, 0xae);
        wait_for_ack(port);
        config = get_byte(port);
    }

    cprintf("SysEx delay is %s.\r\n", (config & CFG_DELAY) ? "enabled" : "disabled");
    cprintf("Fake 'all notes off' is %s.\r\n", (config & CFG_FAKEOFF) ? "enabled" : "disabled");
    cprintf("Gateway version fix is %s.\r\n", (config & CFG_VERFIX) ? "enabled" : "disabled");
    cprintf("Using %s MIDI port.\r\n", (config & CFG_MIDIPT) ? "internal" : "external");
    
    return 0;
}

void send_byte(int port, int stsport, unsigned char byte) {
    time_t timeout = time(NULL) + 2;
    
    while (inp(stsport) & MPU_DRR) {
        if (time(NULL) > timeout) {
            cprintf("ERROR: Timed out waiting for DRR.\r\n");
            exit(1);
        }
    }
    outp(port, byte);
}

unsigned char get_byte(int port) {
    time_t timeout = time(NULL) + 1;

    while (inp(port+1) & MPU_DSR) {
        if (time(NULL) > timeout) {
            cprintf("ERROR: Timed out waiting for DSR.\r\n");
            exit(1);
        }
    }
    return inp(port);
}

void wait_for_ack(int port) {
    time_t timeout = time(NULL) + 4;
    for (;;) {
        while (inp(port+1) & MPU_DSR) {
            if (time(NULL) > timeout) {
                cprintf("ERROR: Timed out waiting for ACK.");
                exit(1);
            }
        }

        if (inp(port) == 0xfe) break;
            
        if (time(NULL) > timeout) {
            cprintf("ERROR: Timed out waiting for ACK.");
            exit(1);
        }
    }
}

void show_help(char *name) {
    cprintf("Initializes the HardMPU card with the given options.\r\n\r\n");
    cprintf("Usage: %s [-p (port)] [-dfgsh]\r\n\r\n");
    cprintf("  -p: Specify port number (default 330)\r\n");
    cprintf("  -d: Enable SysEx delay for MT-32\r\n");
    cprintf("  -f: Fake 'all notes off' for RA-50\r\n");
    cprintf("  -g: Version fix for Gateway\r\n");
    cprintf("  -i: Use internal MIDI port\r\n");
    cprintf("  -s: Show current config, do not set any options\r\n");
    cprintf("  -h: Show this help\r\n"); 
}
