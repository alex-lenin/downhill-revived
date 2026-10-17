/* UNIT 0x00130A88 0x00130C08 */
typedef struct Obj { char pad0[0x6390]; int x6390; char pad6394[0x7720-0x6394]; char x7720[0x7988-0x7720]; unsigned int x7988; char pad798C[0x7ADC-0x798C]; int x7ADC; char pad7AE0[0x7BAD-0x7AE0]; unsigned char x7BAD; } Obj;
extern char D_003B2270[];
extern void func_001D4990(void *);
extern void func_0017B338(void *, int, int, int, int, int);
extern void func_00179A50(void *);
static __inline__ void animate(void *base, unsigned int index, int value) {
    switch (index) { case 0: case 1: case 2: case 3:
        func_0017B338((index * 0x1380 + 0x150) + (char *)base, value, 1, 1, 0, 0); break;
    }
}
static __inline__ void update(void *base, unsigned int index) {
    switch (index) { case 0: case 1: case 2: case 3:
        func_00179A50((index * 0x1380 + 0x150) + (char *)base); break;
    }
}
void func_00130A88(Obj *obj) {
    if (obj->x7BAD == 0) {
        if (obj->x6390 != 0) {
            if (obj->x6390 == 1) {
                obj->x6390 = 0;
                func_001D4990(obj->x7720);
                animate(D_003B2270, obj->x7988, 0x2B);
            } else if (obj->x6390 == 2) {
                obj->x6390 = 1;
                func_001D4990(obj->x7720);
                animate(D_003B2270, obj->x7988, 0x2A);
            } else if (obj->x6390 == 3) {
                obj->x6390 = 2;
                func_001D4990(obj->x7720);
                animate(D_003B2270, obj->x7988, 0x29);
            } else if (obj->x6390 == 4) {
                obj->x6390 = 3;
                func_001D4990(obj->x7720);
                animate(D_003B2270, obj->x7988, 0x28);
                obj->x7ADC = 0;
            }
            update(D_003B2270, obj->x7988);
        }
    }
}
