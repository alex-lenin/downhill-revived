/* UNIT 0x00196590 0x00196820 .rodata=0x28D960:0x28D980 */
typedef struct Effect { int x0; unsigned char x4; char pad5[11]; char x10[16]; } Effect;
typedef struct Rider { char pad0[0x2E94]; struct { float x0,x4,x8; } x2E94; char pad2EA0[0x4098-0x2EA0]; unsigned long x4098; char x40A0[0xE0]; char x4180[0x2490]; char x6610[0x1110]; char x7720[0x20C]; struct { char pad0[16]; char x10[16]; } *x792C; char pad7930[0x58]; unsigned x7988; char pad798C[16]; int x799C,x79A0,x79A4; char pad79A8[0xB4]; int x7A5C; char pad7A60[0xA0]; int x7B00; } Rider;
typedef struct Obj { int x0; unsigned x4; Effect *x8; Rider *xC; int x10; } Obj;
extern struct { char pad[0x10C]; int x10C; } D_004B25A0;
extern struct { char pad[0xBE8]; int xBE8[6]; int padC00; unsigned xC04; } D_004348F0;
extern char D_00398C40[], D_003B2270[], D_004C5640[];
extern struct { char x0; char pad[15]; } D_00398AFC;
extern int func_0022CC08(void *,void *,int,float); extern void func_0017B338(void *,int,int,int,int,int), func_001D4A90(void *,unsigned,void *), func_001C7980(void *), func_00130910(Rider *,int), func_001E67A0(void *), func_001E6420(void *,float), func_00158378(void *), func_001627C8(Rider *), func_001E47B8(void *,unsigned);
static inline char *effect_table(void) { return D_00398C40; }
static inline char *rider_table(void) { return D_003B2270; }
static inline unsigned char rider_flags(Rider *r) { return r->x4098 >> 2; }
static inline void set_setting(Rider *r, unsigned value) { r->x7B00 = value; }
static inline char *trigger_addr(char *table, unsigned index) { return table + (index * 0x1380 + 0x150); }
static inline int duration(unsigned type) { return D_004348F0.xBE8[type]; }
static inline void trigger(char *table, unsigned type, unsigned index) {
    type += 10;
    if (index < 4) func_0017B338(trigger_addr(table,index), type, 1, 0, 0, 0);
}
void func_00196590(Obj *obj, Rider *rider) {
    int color;
    if (!obj->x8->x4) return;
    if (D_004B25A0.x10C == 14) return;
    if (rider_flags(rider) & 1) return;
    if (rider->x7A5C != 1 && rider->x7A5C != 3) return;
    color = 0x202020;
    if (rider->x7A5C == 1 && obj->x4 != 4) {
        trigger(rider_table(), obj->x4, rider->x7988);
    }
    func_001D4A90(effect_table() + 0x240, obj->x4, rider->x792C->x10);
    if (obj->x4 == 5) func_001C7980(rider->x7720);
    switch (obj->x4) {
        case 0: { void *stats = &rider->x2E94; color = 0x20FF20; ((float *)stats)[1] = ((float *)stats)[2]; rider->x799C++; break; }
        case 1: rider->x79A0 = 1; color = 0x20B0FF; break;
        case 2: color = 0x20FFFF; rider->x79A4++; func_00130910(rider,1); func_001E67A0(rider->x6610); break;
        case 3: func_001E6420(rider->x6610,1000.0f); color = 0xFF2010; D_00398AFC.x0 = 1; break;
        case 4: func_00158378(rider->x40A0); color = 0xFF2080; break;
        case 5: color = 0x2020FF; set_setting(rider,D_004348F0.xC04); func_001627C8(rider); break;
    }
    func_0022CC08(obj->x8->x10, rider->x4180, color, 1.0f);
    obj->xC = rider;
    obj->x10 = duration(obj->x4);
    obj->x8->x4 = 0;
    if (D_004B25A0.x10C == 11) func_001E47B8(D_004C5640,obj->x4);
}
