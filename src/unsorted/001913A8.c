/* UNIT 0x001913A8 0x00191528 */
typedef unsigned int U128 __attribute__((mode(TI)));
typedef union Packet { struct { volatile unsigned long tag; unsigned long regs; } h; int words[4]; U128 full; } Packet;
struct Obj { char pad[0x20]; int x20; };
extern char D_00513B00[];
extern void func_0023D618(int, volatile int *, volatile int *);
extern void func_0023D650(int, volatile int *, volatile int *);
extern void func_002105A0(void *, Packet *, int, int, int, int);
extern void func_00210578(void *, Packet *, int, int);

void func_001913A8(struct Obj *obj, Packet *packet, int dx, int dy, int width, int height) {
    volatile int x, y, w, h;
    int halfW, halfH, left, top;
    int id = obj->x20;
    Packet *start = packet;

    packet++;
    func_0023D618(id, &x, &y);
    func_0023D650(id, &w, &h);
    packet->h.tag = 0x4023400000008001;
    packet->h.regs = 0x5151;
    packet++;
    halfH = h >> 1;
    halfW = w >> 1;
    left = x - halfW;
    top = y - halfH;
    left += dx;
    func_002105A0(D_00513B00, packet++, 0xd5, 0x3e, 0x3e, 0x80);
    top += dy;
    func_00210578(D_00513B00, packet++, left, top);
    func_002105A0(D_00513B00, packet++, 0xd5, 0x3e, 0x3e, 0x80);
    func_00210578(D_00513B00, packet++, left + width, top + height);
    start->full = 0;
    start->words[0] = ((int)packet - (int)start) >> 4 | 0x60000000;
}
