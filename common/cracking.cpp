#include "cracking.hpp"
#include <sys/mman.h>

int __nop(int from, int to) {
    mprotect((void *)from, (to-from), PROT_READ | PROT_WRITE | PROT_EXEC);
    int ovr = to - from;
    int i;
    for(i=from; i<to; i++) {
        *(unsigned char *)i = 0x90;
    }
    return ovr; // bytes overwritten
}

void __jmp(int from, int to) {
    mprotect((void *)from, 5, PROT_READ | PROT_WRITE | PROT_EXEC);
    int relative = to - (from+5); // +5 is the position of next opcode
    memset((void *)from, 0xE9, 1); // JMP-OPCODE
    memcpy((void *)(from+1), &relative, 4); // set relative address with endian
}

void __call(int from, int to) {
    mprotect((void *)from, 5, PROT_READ | PROT_WRITE | PROT_EXEC);
    int relative = to - (from+5); // +5 is the position of next opcode
    memcpy((void *)(from+1), &relative, 4); // set relative address with endian
}