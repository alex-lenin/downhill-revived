/* UNIT 0x0021F000 0x0021F0A0 */
typedef struct { long regs[8]; } GSState;
typedef struct { unsigned int en1:1, en2:1, rest:30; } Flags;
void func_0021F000(GSState *states, int index, unsigned int enabled) {
    GSState *p = &states[index];
    ((Flags *)p)->en1 = enabled != 0;
    ((Flags *)p)->en2 = 1;
    *(volatile long *)0x12000000 = p->regs[0];
    *(volatile long *)0x12000020 = p->regs[1];
    *(volatile long *)0x12000090 = p->regs[2];
    *(volatile long *)0x120000A0 = p->regs[3];
    *(volatile long *)0x120000E0 = p->regs[4];
    if (enabled) {
        *(volatile long *)0x12000080 = p->regs[6];
        *(volatile long *)0x12000070 = p->regs[5];
    }
}
