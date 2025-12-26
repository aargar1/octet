#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "../includes/octet.h"

struct termios orig_termios;

void reset_terminal() {
    tcsetattr(0, TCSANOW, &orig_termios);
}

void setup_terminal() {
    tcgetattr(0, &orig_termios);
    atexit(reset_terminal);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &raw);
}

byte pRead(cpu *p, byte *addr) {
    uint16_t a = (uint16_t)(addr - p->mem);

    switch(a) {
        case 0xD010: {
            int c = getchar();
            if (c == EOF) exit(0);
            
            if (c == '\n') c = '\r';
            
            return (byte)c | 0x80;
        }

        case 0xD011:
            return 0x80;

        default:
            return *addr;
    }
}

void pWrite(cpu *p, byte *addr, byte val) {
    uint16_t a = (uint16_t)(addr - p->mem);

    switch(a) {
        case 0xD012: {
            char c = val & 0x7F;
            
            if (c == '\r') c = '\n';
            
            putchar(c);
            fflush(stdout);
            break;
        }
        case 0xD013:
            break;

        default:
            *addr = val;
            break;
    }
}

static void load(cpu *p, const char *file, word addr) {
    FILE *f = fopen(file, "rb");
    if (!f) {
        perror(file);
        exit(1);
    }
    fread(p->mem + addr, 1, 0x10000 - addr, f);
    fclose(f);
}

int main(void) {
    cpu p;
    memset(&p, 0, sizeof(cpu));
    
    p.sp = 0xFF;
    p.flags = 0x20;

    load(&p, "wozmon.bin", 0xFF00);

    p.mem[0xFFFC] = 0x00;
    p.mem[0xFFFD] = 0xFF;

    p.pc = 0xFF00;

    setup_terminal();

    putchar('\n');

    for (;;) {
        pStep(&p);
    }

    return 0;
}