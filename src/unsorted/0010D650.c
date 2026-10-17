/* UNIT 0x0010D650 0x0010D778 */
typedef struct Body {
    char pad0[0x18];
    float x18;
} Body;

typedef struct Rider {
    char pad0[0x40C0];
    char x40C0[0x792C - 0x40C0];
    Body *x792C;
    char pad7930[0x798C - 0x7930];
    int x798C;
} Rider;

typedef struct Obj {
    Rider *x0;
    int x4;
    Rider *x8;
    char padC[0x3A0 - 0xC];
    int x3A0;
} Obj;

typedef struct Global {
    char pad0[0x501C4];
    int x501C4;
} Global;

extern int D_0039E820[];
extern Global D_002AF2C0;
extern float func_001385A8(void *);

int func_0010D650(Obj *obj)
{
    int result = 1;
    float dz;
    float diff;
    float limit;
    int ahead;
    int min = 3;

    if (!((obj->x3A0 >> 11) & 1) && obj->x8 != 0) {
        if (D_0039E820[0] != 0 && obj->x0->x798C >= min) {
            dz = obj->x8->x792C->x18 - obj->x0->x792C->x18;
            diff = func_001385A8(obj->x0->x40C0);
            diff -= func_001385A8(obj->x8->x40C0);
            limit = -50.0f;
            if (diff > 0.0f) {
                limit -= diff * 100.0f;
            }
            ahead = dz < 0.0f && limit < dz;
            if (ahead && D_002AF2C0.x501C4 > min) {
                result = 0;
            }
        }
    }
    return result;
}
