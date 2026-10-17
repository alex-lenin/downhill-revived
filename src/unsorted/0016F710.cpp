/* UNIT 0x0016F710 0x0016F968 */
typedef struct Vec4 {
    float x, y, z, w;
} Vec4 __attribute__((aligned(16)));

class Part {
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual Vec4 *GetPos();
};

struct Thing {
    int type;
    char pad4[0x10 - 0x4];
    Part part;
};

struct Target {
    int type;
    char pad4[0x250 - 0x4];
    char x250[0x480 - 0x250];
    char x480[0x10];
};

struct Anim {
    char pad0[0x14];
    unsigned short flags; /* bit 1 checked */
    short x16;
};

struct Track {
    int x0;
    char pad4[0x8 - 0x4];
    Anim *anim;
};

struct Body {
    Vec4 pos;
    char pad10[0x30 - 0x10];
    Vec4 x30;
    char pad40[0x44 - 0x40];
    float x44;
    char pad48[0x58 - 0x48];
    float x58;
};

struct Obj {
    char pad0[0x10];
    Body body;
    char pad70[0x80 - 0x70];
    Track track;
    char pad8C[0x1F0 - 0x8C];
    Obj *x1F0;
    char pad1F4[0x1FC - 0x1F4];
    int x1FC;
    Target *x200;
    char pad204[0x218 - 0x204];
    float x218;
    float x21C;
    char pad220[0x235 - 0x220];
    unsigned char x235;
};

/* 128-bit vector copy (libvu0-style inline asm, same as func_0016F6E0) */
#define COPY_VEC(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")

extern Thing *func_0016CD78(int, Target *);
extern void func_001DD218(Body *, Vec4 *, Track *, float, float);
extern void func_001DCE58(Body *);
extern void func_0016FC00(Obj *, int);
extern void func_00170160(Obj *);
extern void func_0016EE40(Target *);
extern void func_0016F968(Obj *, void *, float);

void func_0016F770(Obj *obj);

static inline float Max(float a, float b) { return a >? b; }

void func_0016F710(Obj *obj)
{
    if (obj->x200) {
        switch (obj->x200->type) {
        case 4: case 9: case 13: func_0016F770(obj); break;
        }
    }
}

void func_0016F770(Obj *obj)
{
    Body *body;
    Thing *thing;
    Track *track;
    int update;
    Vec4 diff;
    float dist;
    Part *part;
    Vec4 *pos;
    float slack;

    update = 1;
    thing = func_0016CD78(obj->x1FC, obj->x200);
    if (thing) {
        part = &thing->part;
        pos = part->GetPos();
        __asm__ volatile ("lqc2 $vf11, 0x0(%0)\n\tlqc2 $vf12, 0x0(%1)\n\tvsub.xyz $vf11, $vf11, $vf12\n\tsqc2 $vf11, 0x0(%2)"
                          : : "r"(&obj->x1F0->body.pos), "r"(pos), "r"(&diff) : "memory");
        __asm__ volatile ("mula.s %1, %1\n\tmadda.s %2, %2\n\tmadd.s %0, %3, %3\n\tsqrt.s %0, %0"
                          : "=&f"(dist) : "f"(diff.x), "f"(diff.y), "f"(diff.z));
        if (dist < 10.0f) {
            update = 0;
            if (thing->type == 14) {
                obj->x200->type = thing->type;
            }
        }
    }
    if (update) {
        track = &obj->track;
        if (obj->track.x0 && (track->anim->flags >> 1 & 1) && track->anim->x16 == 0) {
            obj->x218 = 0.0f;
        }
        body = &obj->body;
        func_001DD218(body, &diff, track, obj->x218, obj->x21C);
        COPY_VEC(obj->body.x30, &diff);
        func_001DCE58(body);
        COPY_VEC(obj->x1F0->body.pos, body);
        func_0016FC00(obj, 0);
        func_00170160(obj);
        if (obj->track.x0 && (track->anim->flags >> 1 & 1)) {
            obj->x235 = 1;
        }
        if (obj->x235) {
            /* five nested do-while(0) blocks (macro expansions in the original, presumably):
               the loop depth weights body's uses and gives it $s0 ahead of obj */
            do { do { do { do { do {
                slack = body->x58 - 0.1f;
                if (slack > 0.0f) {
                    body->x44 = Max(0.0f, slack);
                } else if (obj->x200) {
                    func_0016EE40(obj->x200);
                }
            } while (0); } while (0); } while (0); } while (0); } while (0);
        }
        if (obj->x200) {
            func_0016F968(obj, obj->x200->x250, -1.5f);
            func_0016F968(obj, obj->x200->x480, 1.5f);
        }
    }
}
