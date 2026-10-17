/* UNIT 0x0015B380 0x0015B460 */
typedef struct Key {
    int x0;
    int x4;
    int x8;
    int xC;
} Key;

typedef struct Obj {
    char pad0[0x4];
    int type;
    char pad8[0x48 - 0x8];
    Key x48;
    char pad58[0x98 - 0x58];
    Key x98;
    char padA8[0x1E2 - 0xA8];
    unsigned char x1E2;
} Obj;

extern int D_0028A340[];
extern float func_00203C28(Key *key);

void func_0015B380(Obj *obj)
{
    Key key;
    float t;
    int value;
    int i;

    key = obj->x48;
    if (obj->type == 7) {
        key = obj->x98;
    }
    t = func_00203C28(&key);
    if (t >= 1.0f) {
        t -= 1.0f;
    }
    value = t * 220.0f;
    for (i = 0; i < 9; i++) {
        if ((unsigned int)(value - D_0028A340[i]) < 6) {
            obj->x1E2 = 1;
            return;
        }
    }
}
