/* UNIT 0x001A9698 0x001A9790 */
typedef struct Point {
    float pos[3];
    float xC;
    float x10;
    float x14;
    char x18;
    signed char x19;
    char pad1A[6];
} Point;

typedef struct Lane {
    char pad0[0x14];
} Lane;

typedef struct Obj {
    char pad0[0x50];
    int count;
    char pad54[4];
    int x58;
    Lane x5C[1];
    char pad70[0x9D0 - 0x70];
    Point points[1];
} Obj;

extern void func_001A87E8(Lane *lane, int index, float max, float min);

void func_001A9698(Obj *obj, float *pos, float a, float b, float c,
                   signed char lane, signed char flag)
{
    if (lane >= 0) {
        obj->points[obj->count].pos[0] = pos[0];
        obj->points[obj->count].pos[1] = pos[1];
        obj->points[obj->count].pos[2] = pos[2];
        obj->points[obj->count].xC = a;
        obj->points[obj->count].x10 = b;
        obj->points[obj->count].x14 = c;
        obj->points[obj->count].x19 = flag;
        func_001A87E8(&obj->x5C[lane], obj->count, a, b);
        if (obj->x58 < lane) {
            obj->x58 = lane;
        }
        obj->count++;
    }
}
