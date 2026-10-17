/* UNIT 0x0018EEC0 0x0018EF08 */
typedef struct Link {
    char pad0[0xF];
    char xF;
    char pad10[0x14 - 0x10];
    void *x14;
    unsigned char x18;
} Link;

extern void func_0018F000(void *owner);
extern void func_0018EF50(void *owner);

void func_0018EEC0(Link *link)
{
    link->xF = 0;
    if (link->x14 != 0) {
        if (link->x18) {
            func_0018F000(link->x14);
        } else {
            func_0018EF50(link->x14);
        }
    }
}
