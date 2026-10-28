/* guest_base for a low host test program (port/tools/testprog.py, host(low=True)): the program
   runs below 2 GiB, so guest address 0 is host address 0 and a host pointer is its own guest
   address, as the guest and host units it links together expect. */
#include <stdint.h>

uintptr_t guest_base;
