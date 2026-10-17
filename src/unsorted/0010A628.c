/* UNIT 0x0010A628 0x0010A6E8 .rodata=0x288880:0x2888C0 */
typedef struct Msg {
    unsigned short type;
} Msg;

typedef struct Handler {
    Msg *msg;
    char pad4[0xC];
    char x10[0x40];
    char x50[0x10];
    char x60;
} Handler;

extern void func_0010A2F8(void *a, void *b, Msg *msg);
extern void func_0010A388(void *a, void *b, Msg *msg);
extern void func_0010A3B0(Msg *msg);
extern void func_0010A3F0(Handler *h, Msg *msg);
extern void func_0010A490(void *a, void *b, Msg *msg);
extern void func_0010A2B8(Msg *msg);

int func_0010A628(Handler *h)
{
    switch (h->msg->type) {
    case 0:
        func_0010A2F8(h->x50, h->x10, h->msg);
        break;
    case 1:
        func_0010A388(h->x50, h->x10, h->msg);
    case 2:
        func_0010A3B0(h->msg);
        break;
    case 9:
        func_0010A3F0(h, h->msg);
        break;
    case 11:
        func_0010A490(h->x50, h->x10, h->msg);
        break;
    case 12:
        func_0010A2B8(h->msg);
        break;
    }
    h->x60 = 0;
    return 0;
}
