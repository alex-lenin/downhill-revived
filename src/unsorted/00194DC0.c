/* UNIT 0x00194DC0 0x00194E10 */
struct Vector { long x; int y; } __attribute__((packed, aligned(4)));
struct Obj { char pad[0x526]; short x526; int x528; struct Vector vectors[30]; };
void func_00194DC0(void *obj, struct Vector *source, int score, int type) {
    int end;
    end=(int)(source+30);
    if (((struct Obj *)obj)->x528 < score) {
        ((struct Obj *)obj)->x528=score;
        ((struct Obj *)obj)->x526=type;
        obj=(char *)obj+0x52c;
        do { *(struct Vector *)obj=*source; ++source; obj=(char *)obj+12; } while((int)source<end);
    }
}
