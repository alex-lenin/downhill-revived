/* UNIT 0x0012C9E0 0x0012CF68 */
typedef struct Obj {
    char pad0[0x2e94];
    char x2E94[0xC];
    char pad1[0x4214 - 0x2ea0];
    float x4214;
    char pad2[0x6390 - 0x4218];
    int x6390;
    char pad3[0x6610 - 0x6394];
    char x6610[0x7988 - 0x6610];
    int x7988;
    char pad4[0x7adc - 0x798c];
    int x7ADC;
    char pad5[0x7b00 - 0x7ae0];
    int x7B00;
    char pad6[0x7ba8 - 0x7b04];
    unsigned char x7BA8, x7BA9, x7BAA, x7BAB;
    char pad7;
    unsigned char x7BAD, x7BAE, x7BAF, x7BB0, x7BB1, x7BB2, x7BB3, x7BB4, x7BB5, x7BB6;
    char pad8;
    int x7BB8;
    int x7BBC;
    int x7BC0;
    int x7BC4;
    int x7BC8;
} Obj;

typedef struct Global2 {
    char pad0[0x12190];
    char x12190[4];
    char pad1[0x74];
    int x12208;
} Global2;
extern Global2 D_004225A0;
extern char D_002AF2C0[];
extern int D_004354F4[];
extern char D_0043EF2C[];

extern int func_0017F050(char *, int);
extern void func_00130910(void *, int);
extern void func_0011C378(void *);
extern void func_0011C400(void *);
extern void func_001627C8(void *);
extern void func_001E61D0(void *);
extern void func_001F0780(void);
extern void func_001F0680(void);
extern void func_0012CF68(void *, int);

void func_0012C9E0(Obj *obj) {
    char *part = (char *)obj + 0xFE0;
    int saved = obj->x7988;
    if (func_0017F050(part, 1)) {
        obj->x7BB8 = 1;
        obj->x7BB3 ^= 1;
    } else if (func_0017F050(part, 3)) {
        func_00130910(obj, 0);
        obj->x7BB8 = 3;
    } else if (func_0017F050(part, 0x16)) {
        func_00130910(obj, 0);
        func_00130910(obj, 0);
        func_00130910(obj, 0);
        func_00130910(obj, 0);
        obj->x7BB8 = 0x16;
    } else if (func_0017F050(part, 4)) {
        obj->x7BB8 = 4;
        obj->x7BA8 ^= 1;
    } else if (func_0017F050(part, 2)) {
        obj->x7BB8 = 2;
        obj->x6390 = 4;
        obj->x7BAD ^= 1;
        if (obj->x7BAD) obj->x7ADC = -1;
        else obj->x7ADC = 3;
    } else if (func_0017F050(part, 5)) {
        obj->x7BB8 = 5;
        obj->x7BAA ^= 1;
        if (obj->x7BAA) obj->x4214 = 1.5f;
        else obj->x4214 = 1.0f;
    } else if (func_0017F050(part, 6)) {
        if (obj->x7BB1 != 0) obj->x7BA9 = 0;
        else obj->x7BA9 ^= 1;
        obj->x7BB8 = 6;
    } else if (func_0017F050(part, 9)) {
        obj->x7BB8 = 9;
        obj->x7BAF ^= 1;
    } else if (func_0017F050(part, 8)) {
        unsigned char b;
        obj->x7BAE ^= 1;
        b = obj->x7BAE;
        D_0043EF2C[0] = b;
        obj->x7BB8 = 8;
    } else if (func_0017F050(part, 7)) {
        obj->x7BB8 = 7;
        obj->x7BAB ^= 1;
    } else {
        if (func_0017F050((char *)obj + 0xFE0, 0xA)) {
            obj->x7BB8 = 0xA;
            func_0011C378(D_002AF2C0);
        } else if (func_0017F050((char *)obj + 0xFE0, 0xB)) {
            obj->x7BB8 = 0xB;
            func_0011C400(D_002AF2C0);
        } else if (func_0017F050((char *)obj + 0xFE0, 0xC)) {
            int i;
            obj->x7BB8 = 0xC;
            for (i = 0; i < 4; i++) D_004225A0.x12190[i] = 1;
        } else if (func_0017F050((char *)obj + 0xFE0, 0xD)) {
            obj->x7BB8 = 0xD;
        } else if (func_0017F050((char *)obj + 0xFE0, 0xE)) {
            obj->x7BB8 = 0xE;
            D_004225A0.x12208 = (D_004225A0.x12208 / 20 + 0x7D0) * 20;
        } else if (func_0017F050((char *)obj + 0xFE0, 0xF)) {
            obj->x7BB8 = 0xF;
            D_004225A0.x12208 = (D_004225A0.x12208 / 20 + 0x1388) * 20;
        } else if (func_0017F050((char *)obj + 0xFE0, 0x10)) {
            obj->x7BB8 = 0x10;
            obj->x7BB0 ^= 1;
        } else if (func_0017F050((char *)obj + 0xFE0, 0x11)) {
            obj->x7BB8 = 0x11;
            obj->x7B00 = D_004354F4[0];
            func_001627C8(obj);
        } else if (func_0017F050((char *)obj + 0xFE0, 0x12)) {
            obj->x7BB8 = 0x12;
            func_001E61D0(obj->x6610);
        } else if (func_0017F050((char *)obj + 0xFE0, 0x13)) {
            obj->x7BB8 = 0x13;
            obj->x7BB2 ^= 1;
            func_001E61D0(obj->x6610);
        } else if (func_0017F050((char *)obj + 0xFE0, 0x14)) {
            float *q;
            obj->x7BB8 = 0x14;
            q = (float *)obj->x2E94;
            q[1] = q[2];
        } else if (func_0017F050((char *)obj + 0xFE0, 0x18)) {
            /* x7BB4 gets the literal 1 (its own register in retail);
               the other flags share this variable. */
            int enable = 1;
            obj->x7BAD = enable;
            obj->x7BB8 = 0x18;
            obj->x7BB4 = 1;
            obj->x7BA8 = enable;
            obj->x6390 = 4;
            obj->x7ADC = -1;
            obj->x7BAA = enable;
            obj->x7BAB = enable;
            obj->x7BB0 = enable;
            obj->x7BB2 = enable;
            obj->x4214 = 1.0f;
            func_001E61D0(obj->x6610);
            obj->x7BB6 = enable;
            func_001F0780();
        } else if (func_0017F050((char *)obj + 0xFE0, 0x19)) {
            obj->x7BB8 = 0x19;
            obj->x7BB5 = 1;
            func_001F0680();
        } else if (func_0017F050((char *)obj + 0xFE0, 0x1A)) {
            obj->x7BB8 = 0x1A;
            obj->x7BB6 ^= 1;
        }
    }
    if (obj->x7BB8 != obj->x7BBC) {
        obj->x7BC0 = 0x78;
        obj->x7BC4 = 1;
        obj->x7BC8 = 0;
    }
    obj->x7BBC = obj->x7BB8;
    func_0012CF68(obj, saved);
}
