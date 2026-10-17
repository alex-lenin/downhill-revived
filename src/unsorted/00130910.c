/* UNIT 0x00130910 0x00130A88 */
typedef struct Obj { char pad[0x6390]; int x6390; char pad6394[0x7988-0x6394]; unsigned int x7988; char pad798c[0x7adc-0x798c]; int x7adc; } Obj;
extern char D_003B2270[];
extern void func_001D48F0(void *);
extern void func_0017B338(void *,int,int,int,int,int);
extern void func_00179A38(void *);
static inline void Play(unsigned int index,int value) {
    char *base = D_003B2270;
    if (index < 4) func_0017B338((index*0x1380+0x150)+base,value,1,1,0,0);
}
static inline void Update(unsigned int index) {
    char *base = D_003B2270;
    if (index < 4) func_00179A38((index*0x1380+0x150)+base);
}
void func_00130910(Obj *obj,int enabled) {
    int active=1;
    if (obj->x6390 == 0) {
        obj->x6390=1;
        if (enabled) {
            func_001D48F0((char *)obj+0x7720);
            Play(obj->x7988,0x24);
        }
    } else if (obj->x6390 == 1) {
        obj->x6390=2;
        if (enabled) {
            func_001D48F0((char *)obj+0x7720);
            Play(obj->x7988,0x25);
        }
    } else if (obj->x6390 == 2) {
        obj->x6390=3;
        if (enabled) {
            func_001D48F0((char *)obj+0x7720);
            Play(obj->x7988,0x26);
        }
    } else if (obj->x6390 == 3) {
        obj->x7adc=3;
        obj->x6390=4;
        if (enabled) {
            func_001D48F0((char *)obj+0x7720);
            Play(obj->x7988,0x27);
        }
    } else if (obj->x6390 == 4) { obj->x7adc=3; active=0; }
    if (active) Update(obj->x7988);
}
