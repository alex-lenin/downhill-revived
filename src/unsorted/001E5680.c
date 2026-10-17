/* UNIT 0x001E5680 0x001E64C8 .sdata=0x29D754:0x29D780 */
#include "include_asm.h"
int D_0029D754 = 0;
float D_0029D758 = 89.0f;
float D_0029D75C = 89.0f;
float D_0029D760 = 280.0f;
float D_0029D764 = 0.25f;
int D_0029D768 = 1800;
int D_0029D76C = 300;
int D_0029D770 = 1500;
float D_0029D774=0.25f;
float D_0029D778=1.0f;
float D_0029D77C = 8.0f;
struct Bike { char pad0[0x796c]; int x796C; char pad7970[8]; int x7978; char pad797c[0xe0]; int x7A5C; };
struct Obj { char pad0[0x38]; float x38; int x3C; struct Bike *x40; };

struct Table { char pad0[0x11a6c]; unsigned char x11A6C[256]; };
extern struct Table D_004225A0;
extern int D_00271B8C[];
void func_001E5680(struct Obj *obj) {
 float scale=1.49f;
 switch(obj->x40->x796C) {
 case 0x20: obj->x38=1.39f; break;
 case 0x40: obj->x38=scale; break;
 case 0x60: obj->x38=1.45f; break;
 case 0x80: obj->x38=1.35f; break;
 case 0xa0: obj->x38=1.25f; break;
 case 0xc0: obj->x38=1.47f; break;
 case 0xe0: obj->x38=1.31f; break;
 case 0x100: obj->x38=1.37f; break;
 case 0x120: obj->x38=1.33f; break;
 case 0x140: obj->x38=1.43f; break;
 case 0x160: obj->x38=1.27f; break;
 case 0x180: obj->x38=1.29f; break;
 case 0x1a0: obj->x38=1.41f; break;
 case 0x200: obj->x38=scale; break;
 default: obj->x38=1.0f; break;
 }
 obj->x3C = (int)((float)D_0029D76C * (obj->x38/scale));
 if (obj->x40 && obj->x40->x7A5C==1) {
 if (D_004225A0.x11A6C[obj->x40->x7978]==2) {D_00271B8C[0]=0x200; return;}
 D_00271B8C[0]=0x800;
 }
}


struct Target_1e5900 { char pad[0x7AC0]; float x7AC0; };
struct Object_1e5900 { char pad[0x40]; struct Target_1e5900 *x40; };

extern int func_001E51A8(void);
extern int func_00138560(void *);
int func_001E5900(struct Object_1e5900 *obj) {
 float value;
 if (!obj->x40) return 0;
 if (func_001E51A8()) return 1;
 if (!(func_00138560((char *)obj->x40 + 0x40C0) & 255)) return 0;
 value = obj->x40->x7AC0;
 if (value < D_0029D77C) { int result = 0; if (value == 0.0f) result = 1; return result; }
 return 1;
}


struct Obj_1e5990 {
    int state;
    int x4;
    char pad8[0x10 - 0x8];
    int x10;
    int x14;
    char pad18[0x71C - 0x18];
    int x71C;
    int x720;
    int x724;
    char pad728[0x10D8 - 0x728];
    int x10D8;
    int x10DC;
    char x10E0;
    char x10E1;
    char x10E2;
    char x10E3;
    char x10E4;
    char pad10E5[0x10EC - 0x10E5];
    int x10EC;
    int x10F0;
    int x10F4;
};

void func_001E5990(struct Obj_1e5990 *obj, int state)
{
    if (obj->state != state) {
        obj->x10E4 = 0;
        obj->state = state;
        obj->x4 = 0;
        obj->x10E2 = 0;
        obj->x10E3 = 0;
        if (state == 0) {
            obj->x10D8 = 0;
            obj->x10DC = 0;
            obj->x724 = 0;
            obj->x720 = 0;
            obj->x10E0 = 0;
            obj->x10E1 = 0;
            obj->x10 = 0;
            obj->x14 = 0;
            obj->x10EC = 0;
            obj->x10F4 = 0;
            obj->x10F0 = 0;
        } else {
            obj->x71C++;
        }
    }
}


typedef struct { int x0, x4, x8; } Record_1e59f8;
typedef struct {
 int x0; int x4; int x8; int xC; float x10, x14, x18, x1C;
 char pad20[0x24]; Record_1e59f8 x44[43]; int x248[43]; int x2F4[43];
 char pad3A0[0x200]; Record_1e59f8 x5A0[30]; int x708, x70C, x710, x714;
 char pad718[0x9CB]; unsigned char x10E3;
} Obj_1e59f8;
extern int func_001E5E58();
void func_001E59F8(Obj_1e59f8 *obj) {
 obj->x1C += (float)func_001E5E58(obj,obj->x0) + obj->x10 + obj->x14;
 obj->x248[obj->x0]++;
 if(obj->x10E3) obj->x2F4[obj->x0]++;
 obj->x44[obj->x714].x0 = obj->x0;
 if(obj->x10E3) obj->x44[obj->x714].x4 = 1;
 obj->x714++;
 if(obj->x70C < 30) {
 obj->x5A0[obj->x70C].x0 = obj->x0;
 obj->x5A0[obj->x70C].x4 = obj->x10E3;
 obj->x5A0[obj->x70C].x8 = 0;
 obj->x70C++;
 }
 obj->x10E3 = 0;
 obj->x8 = obj->x0;
}


typedef struct { int x0,x4,x8; } Event_1e5b20;
typedef struct {
 char pad0[0x3A0]; int counters[8]; int history[30];
 char pad438[0x168]; Event_1e5b20 events[30]; char pad708[4];
 int x70C; char pad710[8]; int x718; char pad71C[0x9DC]; int x10F8;
} Obj_1e5b20;
extern void func_001E5250();
void func_001E5B20(Obj_1e5b20 *obj, int kind) {
 int count;
 obj->counters[kind]++;
 if (kind<5) {
 count=obj->x718;
 if (count<30) { obj->history[count]=kind; obj->x718=count+1; }
 obj->x10F8=kind;
 if (obj->x70C<30) {
 obj->events[obj->x70C].x0=46;
 obj->events[obj->x70C].x4=0;
 obj->events[obj->x70C].x8=kind;
 obj->x70C++;
 }
 } else func_001E5250(obj);
}


typedef struct Obj_1e5bd8 { char pad0[0x18]; float x18; } Obj_1e5bd8;
void func_001E5BD8(Obj_1e5bd8 *obj, int amount) { obj->x18 += (float)amount; }

INCLUDE_ASM("asm/nonmatchings/text", func_001E5BF8);

typedef struct Obj_1e5c00 { char pad0[0x10]; float x10; char pad14[0x14]; float x28; } Obj_1e5c00;
typedef struct Entry_1e5c00 { char pad0[8]; float x8; char padC[8]; } Entry_1e5c00;
struct ScoreEntry { int x0; float x4,x8,xC; int x10; };
extern struct ScoreEntry D_002720C8[];
void func_001E5C00(Obj_1e5c00 *obj, int index) {
    obj->x10 += D_002720C8[index].x8;
    obj->x28 += D_002720C8[index].x8;
}


typedef struct Obj_1e5c40 { char pad[0x10]; float x10; } Obj_1e5c40;
typedef struct Entry_1e5c40 { char pad[8]; float x8; char padC[8]; } Entry_1e5c40;

void func_001E5C40(Obj_1e5c40 *obj, int index) {
    obj->x10 += D_002720C8[index].x8;
}


typedef struct Obj_1e5c68 { char pad[0x14]; float x14; char pad18[8]; float x20; char pad24[0x10E3-0x24]; unsigned char x10E3; } Obj_1e5c68;
extern int func_001E5F38();
void func_001E5C68(Obj_1e5c68 *p, int index) {
 float value, total;
 if(!p->x10E3) {
 value=(float)func_001E5F38(p,index); p->x10E3=1; total=p->x14+value; p->x14=total; p->x20=p->x20+total;
 }
}


typedef struct ButtonCombo_1e5cc8 {
    int x0;
    int buttons[4];
} ButtonCombo_1e5cc8;

typedef struct Rider_1e5cc8 {
    char pad0[0x7978];
    int x7978;
    char pad797C[0x7A5C - 0x797C];
    int x7A5C;
} Rider_1e5cc8;

typedef struct Obj_1e5cc8 {
    char pad0[0x40];
    Rider_1e5cc8 *rider;
} Obj_1e5cc8;

extern ButtonCombo_1e5cc8 D_002719F8[];
extern int func_002205A0(int id);
extern int func_00220230(int handle);

int func_001E5CC8(Obj_1e5cc8 *obj, int combo)
{
    Rider_1e5cc8 *rider;
    int buttons;
    int i;

    rider = obj->rider;
    if (rider->x7A5C != 1) {
        return 0;
    }
    buttons = func_00220230(func_002205A0(rider->x7978));
    for (i = 0; D_002719F8[combo].buttons[i] != 0 && i < 4; i++) {
    }
    i--;
    return (buttons & D_002719F8[combo].buttons[i]) != 0;
}


typedef struct ButtonCombo_1e5d90 {
    int x0;
    int buttons[4];
} ButtonCombo_1e5d90;

typedef struct Rider_1e5d90 {
    char pad0[0x78D4];
    int x78D4;
    char pad78D8[0x7978 - 0x78D8];
    int x7978;
} Rider_1e5d90;

typedef struct Obj_1e5d90 {
    int x0;
    char pad4[0x40 - 0x4];
    Rider_1e5d90 *rider;
    char pad44[0x10EC - 0x44];
    int x10EC;
} Obj_1e5d90;



int func_001E5D90(Obj_1e5d90 *obj, int combo)
{
    Rider_1e5d90 *rider;
    int buttons;
    int i;
    int result;

    rider = obj->rider;
    if (rider->x78D4 == 0) {
        buttons = func_00220230(func_002205A0(rider->x7978)) & 0xF00;
        for (i = 0; D_002719F8[combo].buttons[i] != 0 && i < 4; i++) {
        }
        i--;
        result = buttons != (D_002719F8[combo].buttons[i] & 0xF00);
    } else {
        result = obj->x0 != obj->x10EC;
    }
    return result;
}


struct Obj_1e5e58 { char pad[0x248]; int x248[64]; };
struct Entry_1e5e58 { int x0; float x4; char pad8[12]; };


int func_001E5E58(struct Obj_1e5e58 *obj,int index) {
 float value,penalty;
 int level;
 if((unsigned int)(index-29)<14 || (level=obj->x248[index],penalty=D_0029D774,level==0)) value=D_002720C8[index].x4;
 else {
  if(level==1) value=D_002720C8[index].x4*(1.0f-penalty);
  else if(level==2) value=D_002720C8[index].x4*(1.0f-(penalty+penalty));
  else value=D_002720C8[index].x4*(1.0f-penalty*3.0f);
 }
 return (int)value;
}


typedef struct Entry_1e5f38 {
    char pad0[0xC];
    float xC;
    char pad10[4];
} Entry_1e5f38;

typedef struct Obj_1e5f38 {
    char pad0[0x2F4];
    int x2F4[1];
} Obj_1e5f38;




int func_001E5F38(Obj_1e5f38 *p, int i)
{
    int level = p->x2F4[i];
    float step = D_0029D774;
    float value;

    if (level == 0) {
        value = D_002720C8[i].xC;
    } else if (level == 1) {
        value = D_002720C8[i].xC * (1.0f - step);
    } else if (level == 2) {
        value = D_002720C8[i].xC * (1.0f - (step + step));
    } else {
        value = D_002720C8[i].xC * (1.0f - step * 3.0f);
    }
    return value;
}

struct Inner_1e61d0 {
    char pad0[0x6390];
    int x6390;
    char pad6394[0x7978-0x6394];
    int x7978;
    char pad797c[0x7988-0x797c];
    unsigned int x7988;
    char pad798c[0x7a5c-0x798c];
    int x7A5C;
    char pad7a60[0x7bb2-0x7a60];
    unsigned char x7BB2;
};

struct Obj_1e61d0 {
    char pad0[0xC];
    float xC;
    char pad10[0x1C - 0x10];
    float x1C;
    char pad20[4];
    float x24;
    char pad28[0x2C - 0x28];
    float x2C;
    float x30;
    float x34;
    int x38;
    int x3C;
    struct Inner_1e61d0 *x40;
    char pad44[0x10E5 - 0x44];
    unsigned char x10E5;
    unsigned char x10E6;
    unsigned char x10E7;
};

struct Owner_1e6008 {
    char pad0[0x633C];
    int x633C;
    char pad6340[0x7988 - 0x6340];
    unsigned int x7988;
};

struct Entry_1e6008 {
    int x0;
    int x4;
    int x8;
};

struct Obj_1e6008 {
    char pad0[0xC];
    float xC;
    char pad10[8];
    float x18;
    float x1C;
    char pad20[0xC];
    float x2C;
    char pad30[4];
    float x34;
    char pad38[8];
    struct Owner_1e6008 *x40;
    char pad44[0x3F4];
    struct Entry_1e6008 x438[30];
    struct Entry_1e6008 x5A0[30];
    int x708;
    int x70C;
    int x710;
    char pad714[4];
    int x718;
    int x71C;
    char pad720[0x9C6];
    unsigned char x10E6;
};

extern int func_001EA070(void *p);
extern void func_0012FE38(void *a, void *b);
extern void func_001E6420(struct Obj_1e61d0 *obj, float delta);
extern float func_001E51C0(struct Obj_1e6008 *obj, int count);
extern float func_001E51E8(struct Obj_1e6008 *obj, int count);
extern void func_00179130(void *slot, int amount, int flag);
extern void func_001E61E0(struct Obj_1e61d0 *obj);
extern void func_001E67A8(struct Obj_1e6008 *obj);
extern char D_003B2270[];

void func_001E6008(struct Obj_1e6008 *self)
{
    int delta;
    int i;
    int amount;
    char *base;
    char *cursor;
    unsigned int index;
    float scale;

    if (func_001EA070((char *)self->x40 + 0x5FF0)) {
        func_001E59F8((Obj_1e59f8 *)self);
        func_0012FE38(self->x40, (char *)self->x40 + 0x4ED0);
    }

    delta = self->x40->x633C;
    if (delta != 0) {
        func_001E6420((struct Obj_1e61d0 *)self, (float)delta);
        func_001E5B20((Obj_1e5b20 *)self, 7);
    }
    self->x40->x633C = 0;

    if (self->x1C != 0.0f || self->x18 != 0.0f) {
        self->x1C += self->x18;
        self->x1C *= func_001E51C0(self, self->x718);

        scale = func_001E51E8(self, self->x71C);
        base = D_003B2270;
        self->x1C *= scale;

        self->xC += self->x1C;
        self->x34 += self->x1C;
        amount = (int)self->x1C;

        index = self->x40->x7988;
        if (index < 4)
            func_00179130((index * 0x1380 + 0x150) + base, amount, 1);

        self->x2C += self->x1C;
        if (self->x1C > (float)self->x710) {
            i = 0;
            if (self->x70C > 0) {
                /* copies x5A0[i] to x438[i] for i < x70C */
                cursor = (char *)self + 0x440;
                do {
                    *(int *)(cursor - 8) = *(int *)(cursor + 0x160);
                    *(int *)(cursor - 4) = *(int *)(cursor + 0x164);
                    *(int *)(cursor) = *(int *)(cursor + 0x168);
                    ++i;
                    cursor += 12;
                } while (i < self->x70C);
            }
            self->x710 = (int)self->x1C;
            self->x708 = self->x70C;
        }

        func_001E5250(self);
        func_001E61E0((struct Obj_1e61d0 *)self);
        func_001E67A8(self);
        self->x10E6 = 1;
    }
}

extern void func_00130A88(struct Inner_1e61d0 *obj);

void func_001E61D0(struct Obj_1e61d0 *obj)
{
    obj->x2C = D_0029D768;
}

extern int func_001E6370(struct Obj_1e61d0 *);
extern void func_00130910(struct Inner_1e61d0 *, int);
extern void func_001D4568(void *, int);
extern void func_0017B338(void *, int, int, int, int, int);
extern struct {char pad[0x240]; char x240[16];} D_00398C40;
extern int D_004B2840[];
static inline void Play(unsigned int index) {
 char *base=(char *)D_003B2270;
 if(index<4) func_0017B338((index*0x1380+0x150)+base,0x23,1,1,0,0);
}
static inline void *GetManager(void *base) { return (char *)base+0x240; }
void func_001E61E0(struct Obj_1e61d0 *obj)
{
 float value;
 unsigned int index;
 if (!func_001E5900((struct Object_1e5900 *)obj)) {
  obj->x30 += obj->x1C;
  if (obj->x30 > 0.0f) {
   float *v = (float *)((char *)obj->x40 + 0x2e94);
   v[1] += obj->x30 * 0.2f;
   obj->x30 = 0.0f;
  }
 }
 while ((float)obj->x3C <= obj->x34) {
  func_00130910(obj->x40,1);
  obj->x34 -= (float)obj->x3C;
 }
 value = (float)D_0029D768;
 if (value < obj->x2C) obj->x2C = value;
 if (obj->x40->x7BB2) obj->x2C = (float)D_0029D768;
 if (!obj->x10E7 && func_001E6370(obj) && D_004B2840[0] != 0x1b) {
  if (obj->x40->x7A5C == 1) func_001D4568(GetManager(&D_00398C40),obj->x40->x7978);
  index = obj->x40->x7988;
  Play(index);
 }
 obj->x10E7 = func_001E6370(obj);
}

int func_001E6370(struct Obj_1e61d0 *obj)
{
    int result = 0;

    if (((float)(D_0029D768 - D_0029D76C) <= obj->x2C && obj->x10E5 != 0) || D_0029D754 != 0) {
        result = 1;
    }
    return result;
}

void func_001E63C0(struct Obj_1e61d0 *obj)
{
    struct Inner_1e61d0 *inner = obj->x40;

    if (inner->x6390 != 0) {
        func_00130A88(inner);
    }
}

void func_001E63F0(struct Obj_1e61d0 *obj, float amount)
{
    obj->x24 += amount;
    func_001E6420(obj, amount);
}

void func_001E6420(struct Obj_1e61d0 *obj, float delta)
{
    obj->xC += delta;
    obj->x2C += delta;
    obj->x34 += delta;
    obj->x30 += delta;
    func_001E61E0(obj);
}

static const long D_001E6470 __attribute__((section(".text"))) = 0;

void func_001E6478(void)
{
}

float func_001E6480(struct Obj_1e61d0 *obj)
{
    return obj->x2C;
}

float func_001E6488(void)
{
    return D_0029D770;
}

void func_001E6498(struct Obj_1e61d0 *obj)
{
    float value;

    value = obj->x2C - D_0029D764;
    obj->x2C = value;
    if (value < 0.0f) {
        obj->x2C = 0.0f;
    }
}
