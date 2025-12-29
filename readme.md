# Octet: A Complete 6502 Emulator

<script src="https://asciinema.org/a/5L8u2cUlyWB0M5uXnPclrciLy.js" id="asciicast-5L8u2cUlyWB0M5uXnPclrciLy" async="true"></script>
<!-- [![asciicast](https://asciinema.org/a/5L8u2cUlyWB0M5uXnPclrciLy.svg)](https://asciinema.org/a/5L8u2cUlyWB0M5uXnPclrciLy) -->

The MOS 6502, introduced in 1975, was unbiquitous in home computers and video game systems for the next fifteen years, powering systems from the Apple I to the Super NES, Atari, and Commodore 64. This project is a simple, functional emulator of the NMOS 6502.

> WDC 65C02 (the CMOS 6502) has certain special instructions not fully implemented here.

The emulator does not recreate original clock cycles for the 6502 or 65C02. This is a personal project, not an extremely accurate emulation, but it does pass [Klaus Dormann's extremely thorough test suite](https://github.com/Klaus2m5/6502_65C02_functional_tests/).

## Usage
> For a more complete example, see `tests/apple1.c`.

Octet is designed to be light and portable. Simply include `includes/octet.c` into your project and link with `octet.c`, `opcode.c`, and `processor.c`. A _tiny_ example of Octet's API is given below:

```c
#include "includes/octet.h" /* -or- path to octet.h */
#include <stdlib.h>
#include <unistd.h>

int main (void) {
    cpu p;
    memset(&p, 0, sizeof(cpu));

    p.sp = 0xFF;
    p.flags = 0x20;
    
    p.mem[0xFFFC] = 0x00; /* set reset vectors */
    p.mem[0xFFFD] = 0xFF;
    p.pc = 0xFF00;

    p.mem[0xFF00] = 0x4C; /* JMP $FF00 */
    p.mem[0xFF01] = 0x00;
    p.mem[0xFF02] = 0xFF;

    for (;;) {
        pStep(&p); /* infinite loop */
    }

    return 0;
}
```

## Wozmon
`tests/apple1.c` allows you to emulate Wozmon, the original system monitor of the Apple I. The full source code of Wozmon is contained in `tests/wozmon.s`; after assembly, `wozmon.bin` should be _exactly_ 256 bytes.

The Wozmon interface is rudimentary yet extremely elegant and puts Octet through its paces, testing more than three-fourths of the 6502 instruction set. It allows you to do primarily three things:

1. Peek memory addresses: enter the address or address range into Wozmon and press enter. For example, to peek `$0000`, enter `0000` into Wozmon and press enter; to peek `$0000` through `$0007`, enter `0000.0007` and press enter.

2. Poke memory addresses: just enter the starting address followed by the hex values to be stored. For example, to store `FF FF FF FF` starting at `$0000`, enter `0000: FF FF FF FF` and press enter.

3. Run a program: enter the starting address of the program followed by `R`. For example, if BASIC is loaded at `$0300`, enter `0300R` and press enter.

This current Wozmon source is meant to be compiled with [the _CC65_ suite of compilers](https://cc65.github.io/). See the `makefile` for build instructions (which also assumes the presence of `clang`). Once built, one may simply execute `./build/apple1` and wait for Wozmon's `\` prompt.

> The `apple1.c` harness uses an extremely _rudimentery_ hack to process I/O for Wozmon by intercepting `pRead` and `pWrite` calls and checking for keyboard/display instructions. This leads to unintended behavior when `$D010` through `$D013` are poked. I am sure that there are more elegant ways to route I/O instructions.

## Acknowlegements
Octet was originally created to emulate my own 8-bit computer, inspired by [Ben Eater's series of tutorials on systems architecture](https://eater.net/6502). Skills from [Dr. Chris Terman's MIT 6.004](https://ocw.mit.edu/courses/6-004-computation-structures-spring-2017/) and [The Hebrew University of Jerusalem's _From Nand to Tetris_](https://www.nand2tetris.org/) were invaluable in designing and implementing the emulator. Lastly, [Klaus Dormann's extensive 6502 test suite](https://github.com/Klaus2m5/6502_65C02_functional_tests/) certainly came in clutch for debugging the instruction set.
