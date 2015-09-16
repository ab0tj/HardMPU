#include <conio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <time.h>

#define CFG_DELAY   0x80
#define CFG_FAKEOFF 0x40
#define CFG_VERFIX  0x20
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

    cprintf("\n-- AB0TJ HardMPU Configuration Utility --\n\n");

    while((c = getopt(argc,argv,":p:sfghc")) != -1) {
        switch(c) {
            case 'p':
                port = strtol(optarg,NULL,16);
                break;
            case 's':
                config |= CFG_DELAY;
                break;
            case 'f':
                config |= CFG_FAKEOFF;
                break;
            case 'g':
                config |= CFG_VERFIX;
                break;
            case 'c':
                setconfig = 0;
                break;
            case 'h':
                show_help(basename(argv[0]));
                return 0;
            case '?':
                cprintf("Unknown command line option: -%c\n\n", optopt);
                show_help(basename(argv[0]));
                return 1;
            case ':':
                cprintf("Command line option missing argument: -%c\n\n", optopt);
                show_help(basename(argv[0]));
                return 1;
            default:
                cprintf("Internal error when processing command line options: %c\n", optopt);
                return 1;
        }
    }

    cprintf("Using port 0h%x.\n", port);

    send_byte(port+1, port+1, 0xff);  // reset the mpu

    while (~(inp(port+1)) & MPU_DSR) { // clear any data in the mpu's buffer
        inp(port);
    }

    if (setconfig) {
        send_byte(port+1, port+1, 0xfe);
        wait_for_ack(port);
        send_byte(port, port+1, config);
    } else {
        send_byte(port+1, port+1, 0xae);
        wait_for_ack(port);
        config = get_byte(port);
    }

    cprintf("SysEx delay is %s.\n", (config & CFG_DELAY) ? "enabled" : "disabled");
    cprintf("Fake 'all notes off' is %s.\n", (config & CFG_FAKEOFF) ? "enabled" : "disabled");
    cprintf("Gateway version fix is %s.\n", (config & CFG_VERFIX) ? "enabled" : "disabled");
    
    return 0;
}

void send_byte(int port, int stsport, unsigned char byte) {
    time_t timeout = time(NULL) + 1;
    
    while (inp(stsport) & MPU_DRR) {
        if (time(NULL) > timeout) {
            cprintf("ERROR: Timed out waiting for DRR.\n");
            exit(1);
        }
    }

    outp(port, byte);
}

unsigned char get_byte(int port) {
    time_t timeout = time(NULL) + 1;

    while (inp(port+1) & MPU_DSR) {
        if (time(NULL) > timeout) {
            cprintf("ERROR: Timed out waiting for DSR.\n");
            exit(1);
        }
    }
    
    return inp(port);
}

void wait_for_ack(int port) {
    time_t timeout = time(NULL) + 1;

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
    cprintf("Initializes the HardMPU card with the given options.\n\n");
    cprintf("Usage: %s [-p (port)] [-sfgch]\n\n");
    cprintf("\t-p: Specify port number (default 330)\n");
    cprintf("\t-s: Enable SysEx delay for MT-32\n");
    cprintf("\t-f: Fake 'all notes off' for RA-50\n");
    cprintf("\t-g: Version fix for Gateway\n");
    cprintf("\t-c: Don't set any options, only get current config\n");
    cprintf("\t-h: Show this help\n"); 
}
