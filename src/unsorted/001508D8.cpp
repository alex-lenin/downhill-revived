/* UNIT 0x001508D8 0x001514F8 .rodata=0x28A1E0:0x28A260 */
#include "menu.h"
typedef struct Handle {
    int x0;
    int x4;
    int x8;
    int xC;
} Handle;

typedef struct Part {
    unsigned int x0;
    char pad4[0x1C - 0x4];
    struct Part *x1C;
    struct Part *x20;
} Part;

typedef struct Model {
    char pad0[0xC];
    Part *xC[1];
} Model;

typedef struct Obj {
    char pad0[0x500];
    Model *x500[5];
    int x514[5];
    int x528;
    Part *x52C;
    void *x530;
    void *x534[14];
    void *x56C;
    int x570;
    Handle x574;
} Obj;

struct Game { char pad0[0xABC]; int xABC; };
struct Global { char pad0[0x8]; char x8[0x240 - 0x8]; char x240[0x5B74 - 0x240]; char x5B74[1]; };
extern struct Global D_00398C40;
extern struct Game D_00380D00;
extern char D_003F52D0[];

extern void func_001DDD30(void *p);
extern void func_001C1298(void *p);
extern void func_001B5AF0(void *p, int a, int b);
extern int func_00203590(Handle *, int, int, int);
extern int func_002038E0(Handle *, int);
extern void func_00218498(void *p, int a);
extern void func_00254D08(const char *msg);
extern void func_0018DB08(Obj *obj);


void func_001508D8(Obj *obj)
{
    Handle handle;
    Model *model;
    Part *part;
    Part *sub;
    int idx;
    int i;
    int type;
    struct Global *global;

    global = &D_00398C40;
    func_001DDD30(global->x8);
    func_001C1298(global->x5B74);
    func_001B5AF0(D_003F52D0, 0, 1);
    obj->x528 = 0;
    func_001B5AF0(D_003F52D0, 1, 0);
    func_00203590(&obj->x574, 0x2198, 0, 0);
    handle = obj->x574;
    func_002038E0(&handle, 1);

    if (obj->x530 != 0) {
        switch (D_00380D00.xABC) {
        case 2:
            func_00218498(obj->x530, 0);
            break;
        case 1:
            func_00218498(obj->x530, 1);
            break;
        case 0:
            func_00218498(obj->x530, 2);
            break;
        case 5:
            func_00218498(obj->x530, 3);
            break;
        }
    }
    if (obj->x56C != 0) {
        func_00218498(obj->x56C, obj->x514[0]);
    }

    for (i = 0; i < 5 && (D_00380D00.xABC != 5 || i < 1); i++) {
        model = obj->x500[i];
        if (model != 0) {
            idx = obj->x514[i];
            if (obj->x534[idx] != 0) {
                if (i == 0) {
                    switch (D_00380D00.xABC) {
                    case 4:
                        func_00218498(obj->x534[idx], 3);
                        break;
                    case 3:
                        func_00218498(obj->x534[idx], 1);
                        break;
                    case 0: case 1: case 2: case 5: case 6: case 7: case 8: case 9:
                        func_00218498(obj->x534[idx], 2);
                        break;
                    }
                } else if (i == 1) {
                    func_00218498(obj->x534[idx], 4);
                } else if (i == 2) {
                    func_00218498(obj->x534[idx], 5);
                }
            }
            func_00218498(model, idx);
            part = model->xC[idx];
            if ((part->x0 & 0x3F) != 3) {
                func_00254D08("Wrong type for podium character");
                break;
            }
            sub = part->x1C;
            type = sub->x0 & 0x3F;
            if (type != 1 || (sub->x20->x0 & 0x3F) != type) {
                func_00254D08("Wrong type for podium character");
            } else {
                int mode;
                sub = sub->x20;
                handle.x8 = 0;
                mode = 0;
                switch (i) {
                case 0:
                    mode = 1;
                    obj->x52C = sub;
                    obj->x570 = mode;
                    break;
                case 1:
                case 2:
                    mode = 3;
                    break;
                case 3:
                case 4:
                    mode = 4;
                    break;
                }
                func_00203590(&handle, sub->x0 >> 18, (sub->x0 >> 7) & 0x7FF, mode);
                func_002038E0(&handle, 1);
            }
        }
    }

    if (D_00380D00.xABC == 5) {
        func_00218498(obj->x500[1], 0xE);
        func_00218498(obj->x500[2], 0xE);
    }
    func_0018DB08(obj);
}

typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

typedef struct Handle12 {
    int x0;
    int x4;
    int x8;
} Handle12;

typedef struct Camera {
    char pad0[0x10];
    Vec4 x10;
    char pad20[0x40 - 0x20];
    char x40[1];
} Camera;

typedef struct Obj_150C58 {
    char pad0[0x528];
    int x528;
    unsigned int *x52C;
    char pad530[0x570 - 0x530];
    int x570;
} Obj_150C58;


extern Camera *func_0023D040(int index);
extern void func_00221F50(void *m, Vec4 *rot);
extern void func_0023D1F0(Vec4 *pos, int arg);
extern void func_00150EB8(void *p);
extern void func_00190660(void *a, void *b);
extern bool func_00204230(Handle12 *);
extern void func_0018DC20(Obj_150C58 *obj);
extern void func_001DDDB0(void *p);
extern void func_001D4288(void *p);

void func_00150C58(Obj_150C58 *obj)
{
    Vec4 rot = { 0.10175269f, 0.0f, 3.1218703f };
    Handle12 handle;
    Camera *cam;
    struct Global *global;
    struct { unsigned char v; } stopped;

    cam = func_0023D040(0);
    cam->x10.x = -0.16f;
    cam->x10.y = 13.58f;
    cam->x10.z = 5.37f;
    func_00221F50(cam->x40, &rot);
    func_0023D1F0(&cam->x10, 0);

    if (obj->x528 >= 0x1A5) {
        func_00150EB8(&D_00380D00);
        func_00190660(D_003F52D0, &D_00380D00);
    }

    if (obj->x570 != 0) {
        handle.x8 = 0;
        func_00203590((Handle *)&handle, *obj->x52C >> 18, (*obj->x52C >> 7) & 0x7FF, 1);
        stopped.v = !func_00204230(&handle);
        if (stopped.v) {
            func_00203590((Handle *)&handle, *obj->x52C >> 18, (*obj->x52C >> 7) & 0x7FF, 2);
            func_002038E0((Handle *)&handle, 1);
            obj->x570 = 0;
        }
    }

    obj->x528++;
    func_0018DC20(obj);
    global = &D_00398C40;
    func_001DDDB0(global->x8);
    func_001D4288(global->x240);
}

typedef struct Item_150DD0 {
    unsigned int flags; /* bits 7..17 = slot index */
} Item_150DD0;

typedef struct Obj_150DD0 {
    char pad0[0x500];
    Item_150DD0 *x500[5];
} Obj_150DD0;

void func_00150DD0(Obj_150DD0 *self, Item_150DD0 *item)
{
    unsigned int slot = (item->flags >> 7) & 0x7FF;
    if (slot < 5) {
        self->x500[slot] = item;
    }
}

struct S_00150DF8 {
    char pad[0x530];
    int x530;
};

void func_00150DF8(struct S_00150DF8 *s, int v) {
    s->x530 = v;
}

struct S_00150E00 {
    char pad[0x56C];
    int x56C;
};

void func_00150E00(struct S_00150E00 *s, int v) {
    s->x56C = v;
}

typedef struct Obj_150E08 {
    char pad0[0x534];
    void *x534[20];
    unsigned int count;
} Obj_150E08;

void func_00150E08(Obj_150E08 *self, void *item)
{
    unsigned int n = self->count;
    if (n < 14) {
        self->x534[n] = item;
        self->count = n + 1;
    }
}

struct Obj_150E30 {
    char pad0[0x514];
    int x514[5];
};

void func_00150E30(struct Obj_150E30 *obj, int value, unsigned int index) {
    if (index < 5) {
        obj->x514[index] = value;
        if (value >= 14) {
            obj->x514[index] = 13;
        }
    }
}

extern char D_004B2914[];
extern void func_001C5B80(void *a);

void func_00150E60(void)
{
    func_001C5B80(D_004B2914);
}

typedef struct Obj_150E80 { char pad[0x4F8]; void *x4F8; } Obj_150E80;
extern char D_00284338[];

Obj_150E80 *func_00150E80(Obj_150E80 *obj) {
    InitializeMenuDiscard((SharedMenu *)(obj));
    obj->x4F8 = D_00284338;
    return obj;
}

typedef struct { char pad0[0xA17]; unsigned char xA17,xA18,xA19; char padA1A[0xA]; int xA24,xA28,xA2C,xA30,xA34,xA38,xA3C,xA40; } Obj_150EB8;
typedef struct { int x0,x4,x8,xC; void *x10,*x14,*x18; int x1C; void *x20,*x24; } Table_150EB8;
extern Table_150EB8 D_0026E670;
extern char D_00277EC0[];
extern void *func_00158638(int);
extern void func_001517A0(Obj_150EB8 *,int,void *);
void func_00150EB8(Obj_150EB8 *o) {
 
 o->xA3C=300; o->xA30=0; o->xA40=0;
 if(o->xA28) {
 int i,next,kind,special;
 i=0;
 do {
 next=i+1;
 if((o->xA28>>i)&1) {
 kind=next<<5; special=0;
 if(kind==0xE0 || kind==0x100 || kind==0x140 || kind==0x180 || kind==0x1A0) special=1;
 if(special) func_001517A0(o,D_0026E670.x4,func_00158638(i));
 else func_001517A0(o,D_0026E670.xC,func_00158638(i));
 }
 i=next;
 }while(i<18);
 }
 if(o->xA24) {
 int i;
 i=0;
 do { if((o->xA24>>i)&1) func_001517A0(o,D_0026E670.x8,func_00158638(i)); ++i; } while(i<18);
 }
 if(o->xA2C) {
 int i; char *entry; Table_150EB8 *table=&D_0026E670;
 entry=D_00277EC0; i=0;
 do { if((o->xA2C>>i)&1) func_001517A0(o,table->x1C,entry); ++i; entry+=16; } while(i<18);
 }
 if(o->xA19) func_001517A0(o,D_0026E670.x0,D_0026E670.x10);
 if(o->xA18) func_001517A0(o,D_0026E670.x0,D_0026E670.x14);
 if(o->xA17) func_001517A0(o,D_0026E670.x0,D_0026E670.x18);
 if(o->xA34) func_001517A0(o,D_0026E670.x0,D_0026E670.x20);
 if(o->xA38) func_001517A0(o,D_0026E670.x0,D_0026E670.x24);
}

struct Obj_151120 { char pad[0x418]; int x418; };
extern void func_0018DB08(struct Obj_151120 *);
void func_00151120(struct Obj_151120 *obj) {
    obj->x418 = 0;
    func_001B5AF0(D_003F52D0, 1, 1);
    func_0018DB08(obj);
}

struct Menu {
    char pad0[0xA30];
    int xA30;
    char padA34[0xA40 - 0xA34];
    int xA40;
};

extern void func_0023D650(int, int *, int *);
extern void func_0020FB80(int, int, int, int, int);
extern void func_0020F518(int, int, int, int, int);
extern int func_001514F8(struct Menu *, unsigned char *);
extern void func_00151280(struct Menu *, int, int *, int);
extern void func_001517D0(struct Menu *);
extern void func_001516D8(struct Menu *);
extern void func_0018DC20(struct Menu *);
extern void func_001D4288(void *);
extern void func_001DDDB0(void *);

void func_00151160(struct Menu *menu)
{
    unsigned char flag;
    int width;
    int height;
    int y;
    int x;
    int result;
    struct Global *global;

    func_0023D650(0, &width, &height);
    y = 0x28;
    x = width / 2;
    func_0020FB80(0, 0x180, 0x120, -0x7B, -0x20);
    func_0020F518(0, 0x8C, 0x8C, 0x8C, 0x8C);
    result = func_001514F8(menu, &flag);
    if (flag) func_00151280(menu, x, &y, 1);
    func_00151280(menu, x, &y, result);
    if (menu->xA40 < menu->xA30) func_001517D0(menu);
    else func_001516D8(menu);
    func_0018DC20(menu);
    global = &D_00398C40;
    func_001D4288(global->x240);
    func_001DDDB0(global->x8);
}

extern char *func_001E6E80(int);
extern void func_001E6EB0(int,int *);
extern void func_0020F9F0(int,int,int,int,char *);
extern void func_00254544(char *,char *);
extern void func_002542C4(char *,char *);
extern char D_0029C600[],D_0029C608[],D_0029C610[],D_0029C618[],D_0029C620[],D_0029C628[],D_0029C630[],D_0029C638[],D_0029C640[],D_0029C648[];
void func_00151280(int unused,int x,int *y,int code) {
    char label[32];
    int codes[8];
    char text[32];
    int i;
    if(code==1) { __builtin_memcpy(label,"Unlock Code",12); }
    else func_00254544(label,func_001E6E80(code));
    func_0020F9F0(0,0,x,*y,label);
    func_001E6EB0(code,codes);
    *y+=15;
    text[0]=D_0029C600[0];
    for(i=0;i<8 && codes[i]!=0;i++) {
        switch(codes[i]) {
        case 0x10: func_002542C4(text,D_0029C608); break;
        case 0x40: func_002542C4(text,D_0029C610); break;
        case 0x80: func_002542C4(text,D_0029C618); break;
        case 0x20: func_002542C4(text,D_0029C620); break;
        case 0x8000: func_002542C4(text,D_0029C628); break;
        case 0x1000: func_002542C4(text,D_0029C630); break;
        case 0x2000: func_002542C4(text,D_0029C638); break;
        case 0x4000: func_002542C4(text,D_0029C640); break;
        }
        func_002542C4(text,D_0029C648);
    }
    func_0020F9F0(0,0,x,*y,text);
    *y+=15;
}
