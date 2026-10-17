/* UNIT 0x0018F9C0 0x0018FB68 */
typedef unsigned int U128 __attribute__((mode(TI)));
typedef union Packet {
    struct { volatile unsigned long tag; unsigned long regs; } h;
    int words[4];
    U128 full;
} Packet;
struct Obj { char pad[0x20]; int x20; };

extern struct Obj D_003F52D0;
extern Packet D_003F4D30[];
extern char D_00513B00[];
extern void func_0023D618(int, int *, int *);
extern void func_0023D650(int, int *, int *);
extern void func_002105A0(void *, Packet *, int, int, int, int);
extern void func_00210578(void *, Packet *, int, int);
extern void func_002103D8(void *, int, Packet *);

void func_0018F9C0(int unused, int dx, int dy, int width, int height) {
    int x, y, w, h;
    int halfW, halfH, left, top;
    int id = D_003F52D0.x20;
    Packet *start;
    Packet *packet;

    func_0023D618(id, &x, &y);
    func_0023D650(id, &w, &h);
    /* start is copied from packet (not from D_003F4D30) so the packet
       pointer is not rewritten as start+offset; start then stays a
       spilled constant and every use rematerializes the address */
    packet = D_003F4D30;
    start = packet;
    packet++;
    packet->h.tag = 0x4003400000008001;
    packet->h.regs = 0x5151;
    packet++;
    halfW = w >> 1;
    halfH = h >> 1;
    left = x - halfW;
    top = y - halfH;
    left += dx;
    func_002105A0(D_00513B00, packet++, 0xDD, 0xD1, 10, 128);
    top += dy;
    func_00210578(D_00513B00, packet++, left, top);
    func_002105A0(D_00513B00, packet++, 0xDD, 0xD1, 10, 128);
    func_00210578(D_00513B00, packet++, left + width, top + height);
    start->full = 0;
    start->words[0] = ((int)packet - (int)start) >> 4 | 0x60000000;
    func_002103D8(D_00513B00, D_003F52D0.x20, start);
}
