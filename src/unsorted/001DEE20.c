/* UNIT 0x001DEE20 0x001DF560 */
#include "scalar_api.h"
#include "sound.h"
typedef struct Obj {
    char pad0[0x54];
    int x54;
    int x58;
    char pad5C[0x8C - 0x5C];
    int x8C;
    int x90;
    char pad94[0xC4 - 0x94];
    int xC4;
    unsigned int xC8;
    int xCC[40];
    char pad16C[0x17C - 0x16C];
    int x17C;
    int x180;
    int x184;
    int x188;
    int x18C;
    int x190;
    int x194;
    int x198;
    char pad19C[0x1C8 - 0x19C];
    unsigned int x1C8[10];
} Obj;

typedef struct State43 {
    int x0;
    signed char x4[0xE];
    signed char x12;
    signed char x13;
    int x14[4];
    signed char x24[0x28];
    int x4C;
} State43;

typedef struct Rider {
    char pad[0x7A68];
    unsigned char x7A68;
} Rider;

typedef struct Global2A {
    char pad[0x50130];
    Rider *riders[1];
} Global2A;

extern State43 D_0043DCC0;
extern Global2A D_002AF2C0;
extern char D_003B2270[];
extern int D_0043F348[];
extern int func_002013F8(void);
extern void func_001DF600(unsigned int *);
extern void func_001DF638(Obj *);
extern void func_001DF820(Obj *);
extern int func_001DEDE0(void *);
extern int func_001DEDA0(void *);
extern void func_001DE918(Obj *);

#if defined(__mips__)
extern void func_00202688(int, int, int, int, int, int, int, int, int, int, void (*)(void), unsigned long);
#else

#endif

extern int func_00222B08(int, int);
extern void func_00177AA0(void *, int);

void func_001DEE20(Obj *obj)
{
    int tries;
    int selected;
    int handle;
    int next_index;

    func_002013F8();
    if ((obj->x180 ^ D_0043DCC0.x12) != 0) {
        if (obj->xC8 == 0xFFFFFFFF) {
            obj->x17C = -1;
        } else if (obj->xC8 == 0) {
            obj->x17C = -1;
        } else {
            func_001DF600(&obj->xC8);
            obj->x17C = -1;
        }
    } else {
        if (D_0043DCC0.x14[D_0043DCC0.x12] != 0) {
            D_0043DCC0.x14[D_0043DCC0.x12] = 0;
            if (obj->x184 != D_0043DCC0.x4[obj->x17C]) {
                if (obj->xC8 != 0xFFFFFFFF && obj->xC8 != 0) {
                    func_001DF600(&obj->xC8);
                    while (func_002013F8()) {
                    }
                    obj->xC4 = 0;
                }
            }
        }
    }
    if (D_0043DCC0.x13 != 0 && D_0043DCC0.x0 > 0) {
        func_001DF638(obj);
        if (obj->x1C8[obj->x190] != 0xFFFFFFFF && obj->x1C8[obj->x190] != 0) {
            func_001DF600(&obj->x1C8[obj->x190]);
            obj->x1C8[obj->x190] = 0;
        }
        if (obj->x1C8[obj->x18C] != 0xFFFFFFFF && obj->x1C8[obj->x18C] != 0) {
            func_001DF600(&obj->x1C8[obj->x18C]);
            obj->x1C8[obj->x18C] = 0;
        }
        if (obj->x1C8[obj->x194] != 0xFFFFFFFF && obj->x1C8[obj->x194] != 0) {
            func_001DF600(&obj->x1C8[obj->x194]);
            obj->x1C8[obj->x194] = 0;
        }
        if (D_0043DCC0.x4C == 0) {
            if (obj->x58 != 0 && (unsigned)(D_0043F348[0] - 2) < 3 && D_002AF2C0.riders[0]->x7A68 == 0) {
                char *p;
                int v;
                p = D_003B2270;
                obj->x58 = 0;
                v = D_0043DCC0.x4[obj->x17C];
                obj->x184 = v;
                func_00177AA0(p + 0x150, v);
            }
            if (obj->xC8 == 0) {
                if (obj->xC4 == 1) {
                    obj->x17C++;
                    if (obj->x17C >= D_0043DCC0.x0) {
                        obj->x17C = 0;
                    }
                } else {
                    obj->xC4 = 1;
                }
                obj->xC8 = 0xFFFFFFFF;
                func_00202688(obj->xCC[D_0043DCC0.x4[obj->x17C]], 0, 0, 0, 0x400, 0, 1, 0, 0, 0, func_00208C50, (unsigned long)(unsigned int)&obj->xC8);
                obj->x58 = 1;
            } else {
                handle = obj->xC8;
                if (handle != 0xFFFFFFFF && handle != 0) {
                    obj->xC8 = 0xFFFFFFFF;
                    func_00201D80(handle, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->xC8));
                }
            }
        } else {
            if (obj->x58 != 0 && (unsigned)(D_0043F348[0] - 2) < 3 && D_002AF2C0.riders[0]->x7A68 == 0) {
                char *p;
                p = D_003B2270;
                obj->x58 = 0;
                obj->x184 = obj->x17C;
                func_00177AA0(p + 0x150, obj->x17C);
            }
            if (obj->xC8 == 0) {
                tries = 0;
                selected = func_00222B08(0, 0x27);
                obj->x17C = selected;
                while (D_0043DCC0.x24[selected] == 0 || selected == obj->x90) {
                    tries++;
                    obj->x17C = func_00222B08(0, 0x27);
                    if (tries >= 6) {
                        obj->x17C = obj->x90 + 1;
                        if (obj->x17C >= 0x28) {
                            obj->x17C = 0;
                        }
                        if (D_0043DCC0.x24[obj->x17C] == 0) {
                            do {
                                obj->x17C++;
                                if (obj->x17C >= 0x28) {
                                    obj->x17C = 0;
                                }
                            } while (D_0043DCC0.x24[obj->x17C] == 0);
                        }
                    }
                    selected = obj->x17C;
                }
                obj->xC8 = 0xFFFFFFFF;
                obj->x90 = obj->x17C;
                func_00202688(obj->xCC[obj->x17C], 0, 0, 0, 0x400, 0, 1, 0, 0, 0, func_00208C50, (unsigned long)(unsigned int)&obj->xC8);
                obj->x58 = 1;
            } else {
                handle = obj->xC8;
                if (handle != 0xFFFFFFFF && handle != 0) {
                    obj->xC8 = 0xFFFFFFFF;
                    func_00201D80(handle, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->xC8));
                }
            }
        }
    } else {
        if (obj->xC8 != 0xFFFFFFFF && obj->xC8 != 0) {
            func_001DF600(&obj->xC8);
        }
        obj->x17C = -1;
        if (func_001DEDE0(obj) != 0) {
            if ((unsigned)(D_0043F348[0] - 1) < 3) {
                func_001DF820(obj);
            }
            if (obj->x190 != obj->x18C && obj->x198 == 0) {
                obj->x8C = 1;
                obj->x198 = 1;
            }
            func_001DE918(obj);
        } else {
            if (obj->x1C8[obj->x190] == 0) {
                if (func_001DEDA0(obj) != 0) {
                    tries = 0;
                    obj->x54 = func_00222B08(0, 9);
                    if (obj->x54 == obj->x90) {
                        do {
                            tries++;
                            obj->x54 = func_00222B08(0, 9);
                            if (tries >= 4) {
                                next_index = obj->x90 + 1;
                                obj->x54 = next_index;
                                if (next_index >= 10) {
                                    obj->x54 = 0;
                                }
                            }
                        } while (obj->x54 == obj->x90);
                    }
                } else {
                    tries = 0;
                    obj->x54 = func_00222B08(10, 0x13);
                    if (obj->x54 == obj->x90) {
                        do {
                            tries++;
                            obj->x54 = func_00222B08(10, 0x13);
                            if (tries >= 4) {
                                next_index = obj->x90 + 1;
                                obj->x54 = next_index;
                                if (next_index >= 0x14) {
                                    obj->x54 = 10;
                                }
                            }
                        } while (obj->x54 == obj->x90);
                    }
                }
                obj->x90 = obj->x54;
                obj->x1C8[obj->x190] = 0xFFFFFFFF;
                func_00202688(obj->xCC[obj->x54], 0, 0, 0, 0x400, 0, 1, 0, 0, 0, func_00208C50, (unsigned long)(unsigned int)&obj->x1C8[obj->x190]);
            } else {
                handle = obj->x1C8[obj->x190];
                if (handle != 0xFFFFFFFF && handle != 0) {
                    obj->x1C8[obj->x190] = 0xFFFFFFFF;
                    func_00201D80(handle, (SoundCompletionCallback)(func_00208C50), (SoundCallbackData)(&obj->x1C8[obj->x190]));
                }
            }
        }
    }
    obj->x180 = D_0043DCC0.x12;
}
