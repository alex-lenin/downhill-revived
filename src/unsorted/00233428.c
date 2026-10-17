/* UNIT 0x00233428 0x00233488 */
typedef struct { char pad[0x18C]; int x18C[17]; int x1D0; } Obj;
typedef struct { int x0; unsigned short x4; char pad[0x12]; int x18; int x1C[17]; } Header;
int func_00233428(Obj *obj, Header *header) {
    int *src, *dst, count;
    if (header->x0 == 0x58454B53 && header->x4 == 0x1070) {
        src = header->x1C;
        dst = obj->x18C;
        count = 16;
        obj->x1D0 = header->x18;
        do { *dst++ = *src++; count--; } while (count >= 0);
        return 0;
    }
    return 1;
}
