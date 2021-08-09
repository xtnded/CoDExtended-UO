#ifndef _CRACKING_HPP_
#define _CRACKING_HPP_

extern "C" {
    #include <string.h>

    int __nop(int from, int to);
    void __jmp(int from, int to);
    void __call(int from, int to);
}

#endif