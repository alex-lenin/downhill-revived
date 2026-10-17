/* UNIT 0x001217E8 0x00122098 .sdata=0x29C180:0x29C198 */
typedef struct Vec { float x,y,z,w; } Vec;
typedef unsigned int U128 __attribute__((mode(TI)));
typedef struct VTable { char p0[0x20]; short d20; short p22; Vec *(*f24)(void *); char p28[8]; short d30; short p32; Vec *(*f34)(void *); } VTable;
typedef struct Model { unsigned int *x0; char p4[0xC]; Vec x10; char p20[0x20]; Vec x40,x50,x60; } Model;
typedef struct Obj { VTable *vt; int x4; char p8[8]; Model *x10; char *x14; char p18[0x1C]; void *x34; void *x38; char p3C[0x30]; Vec x6C; char p7C[0x30]; Vec xAC,xBC,xCC,xDC,xEC; char pFC[0x74]; Vec x170; char p180[0x28]; char x1A8[0xC0]; unsigned long x268; } Obj;
typedef struct Rider { char p0[0x40C0]; char x40C0[0x60]; Vec x4120; char p4130[0x40]; float x4170; char p4174[0xC]; Vec x4180; char p4190[0x379C]; Model *x792C; } Rider;
typedef struct Contact { char p0[0x20]; Vec x20; } Contact;
typedef struct Effect { char p0[9]; unsigned char x9; char pA[0x5A]; } Effect;
typedef struct Pool { Effect effects[32]; unsigned char indices[32]; unsigned char count; } Pool;
int D_0029C180 = 0;
float D_0029C184 = 4000.0f;
float D_0029C188 = 6000.0f;
float D_0029C18C = 1.0f;
float D_0029C190 = 25.0f;
float D_0029C194 = 30.0f;
extern struct { char p0[0x1230]; char x1230[16]; } D_00398C40;
extern char D_0038DCE0[];
extern Pool D_004A9160;
extern void func_001BE158(void *,Vec *);
extern void func_001CFA48(void *,Vec *,int);
extern void *func_00164A10(void *);
extern void func_0011E568(Obj *,int);
extern void func_00254204(void *,int,int);
extern void func_00164EE8(void *,int,Vec *,Vec *,void *);
extern void func_00164FF0(void *,Rider *,Contact *);
extern int func_00238F48(void);
extern void func_0012F110(void *,Obj *,float,float,float);
extern void func_00221888(Vec *,Vec *,Vec *);
extern float func_00203C28(Vec *);
extern void func_001AC7B0(Effect *,Vec *);
extern void func_001ACB70(Effect *,float);
extern void func_001AC920(Effect *,float,float);
extern void func_0012F328(Rider *,Contact *,float,float);
extern float func_00235EA8(float,float,float,float);
#define GETPOS(o) ((o)->vt->f24((char *)(o)+(o)->vt->d20))
#define GETVEC(o) ((o)->vt->f34((char *)(o)+(o)->vt->d30))
#define COPY_VECTOR(dst,src) __asm__ volatile("lq $2,0(%1)\n\tsq $2,%0" : "=m"(dst) : "r"(src) : "$2")
#define SUB(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvsub.xyz $vf11,$vf11,$vf12\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define CROSS(dst,a,b) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tvopmula.xyz ACC,$vf11,$vf12\n\tvopmsub.xyz $vf11,$vf12,$vf11\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b))
#define SCALE(dst,a,s) __asm__ volatile("lqc2 $vf11,0(%1)\n\tmfc1 $2,%2\n\tqmtc2 $2,$vf12\n\tvmulx.xyz $vf11,$vf11,$vf12x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"f"(s) : "$2")
#define MADD(dst,a,b,s) __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2 $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0" : "=m"(dst) : "r"(a),"r"(b),"f"(s) : "$2")
#define DOT(out,a,b) __asm__ volatile("mula.s %1,%2\n\tmadda.s %3,%4\n\tmadd.s %0,%5,%6" : "=f"(out) : "f"((a).x),"f"((b).x),"f"((a).y),"f"((b).y),"f"((a).z),"f"((b).z))
#define LENGTH(out,x,y,z) __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3\n\tsqrt.s %0,%0" : "=&f"(out) : "f"(x),"f"(y),"f"(z))

static inline void set_vec(Vec *v,float x,float y,float z,float w) { v->x=x; v->y=y; v->z=z; v->w=w; }
static inline void *get_global(void) { return &D_00398C40; }
static inline int get_count(Pool *pool) { return pool->count; }
static inline Effect *alloc_effect(Pool *pool) { unsigned char n=get_count(pool); if(n<32) { pool->count++; return &pool->effects[pool->indices[n]]; } return 0; }
static inline float dot_vec(Vec *a,Vec *b) { float r; DOT(r,*a,*b); return r; }
static inline float normalize(Vec *dst,Vec *src) { float x=src->x,y=src->y,z=src->z,len,inv; __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len) : "f"(x),"f"(y),"f"(z) : "$2"); dst->x=x*inv; dst->y=y*inv; dst->z=z*inv; return len; }
static inline float normalize_length(Vec *dst,Vec *src) { float x=src->x,y=src->y,z=src->z,len,inv; __asm__ volatile("mula.s %2,%2\n\tmadda.s %3,%3\n\tmadd.s %1,%4,%4\n\tsqrt.s %1,%1\n\tlui $2,0x3f80\n\tmtc1 $2,%0\n\tnop\n\tdiv.s %0,%0,%1" : "=&f"(inv),"=&f"(len) : "f"(x),"f"(y),"f"(z) : "$2"); dst->x=x*inv; dst->y=y*inv; dst->z=z*inv; return len; }
static inline void set_type(Effect *e,int value) { e->x9=value; }
static inline void EFFECT(Effect *e, Vec *p) { func_001AC7B0(e,p); set_type(e,4); func_001ACB70(e,1.5f); func_001AC920(e,100.0f,100.0f); }
typedef struct Physics { char p0[0x150]; float mass; } Physics;
static inline Physics *get_physics(Rider *r) { return (Physics *)r->x40C0; }
void func_001217E8(Obj *obj,Rider *rider,Contact *contact)
{
    struct { union { Vec v[3]; void *args[12]; } u; Vec v30,v40,v50,v60; } st __attribute__((aligned(16)));
    float length,factor,w,one;
    Vec *a,*b,*pos,*tmp;
    Model *model;
    Physics *phys;
    Effect *effect;
    if (!rider) return;
    func_001BE158((char *)get_global()+0x1230,GETPOS(obj));
    func_001CFA48(obj->x1A8,&obj->x10->x10,obj->x4);
    switch (*obj->x10->x0 >> 18) {
    case 0x4B0: case 0x4B5: break;
    case 0x4C9:
        if(!obj->x34) {
            obj->x34=func_00164A10(D_0038DCE0);
            if(obj->x34) {
                func_0011E568(obj,7);
                func_00254204(st.u.args,0,0x28);
                st.u.args[0]=&obj->x6C;
                set_vec(&st.v30,4.0f,4.0f,0.0f,0.0f);
                func_00164EE8(obj->x34,4,GETPOS(obj),&st.v30,st.u.args);
                if(obj->x14) obj->x14[4]=0;
            }
        }
        func_00164FF0(obj->x34,rider,contact);
        break;
    case 0x4B8: case 0x4D3: case 0x5D1: case 0x5D3:
        if(!obj->x34) {
            obj->x34=func_00164A10(D_0038DCE0);
            if(obj->x34) {
                func_0011E568(obj,7);
                func_00254204(st.u.args,0,0x28);
                st.u.args[0]=&obj->xAC; st.u.args[1]=&obj->xBC; st.u.args[3]=&obj->xCC;
                set_vec(&st.v40,4.0f,4.0f,0.0f,0.0f);
                if((*obj->x10->x0 & 0xFFFC0000)==0x12E00000)
                    func_00164EE8(obj->x34,1,GETPOS(obj),&st.v40,st.u.args);
                else
                    func_00164EE8(obj->x34,5,GETPOS(obj),&st.v40,st.u.args);
            }
        }
        func_00164FF0(obj->x34,rider,contact);
        break;
    case 0x4CB: case 0x4E3: case 0x4E5: case 0x5CD: case 0x5CF:
        if(!obj->x34) {
            obj->x34=func_00164A10(D_0038DCE0);
            if(obj->x34) {
                func_0011E568(obj,7);
                func_00254204(st.u.args,0,0x28);
                st.u.args[0]=&obj->xAC; st.u.args[1]=&obj->xBC; st.u.args[3]=&obj->xCC;
                set_vec(&st.v30,4.0f,4.0f,0.0f,0.0f);
                func_00164EE8(obj->x34,6,GETPOS(obj),&st.v30,st.u.args);
            }
        }
        func_00164FF0(obj->x34,rider,contact);
        break;
    case 0x4BA:
        if(func_00238F48()-D_0029C180 >= 61) {
            func_0012F110(obj->x38,obj,D_0029C184,D_0029C188,D_0029C18C);
            a=&rider->x792C->x10; b=GETVEC(obj);
            SUB(st.u.v[0],a,b);
            func_00221888(&obj->x10->x40,&st.u.v[0],&st.u.v[1]);
            if(st.u.v[1].x>0.0f) {
                float result;
                st.u.v[2]=obj->xEC;
                result=func_00203C28(&st.u.v[2]);
                if(result>=0.99f || result<=0.0f) {
                    effect=alloc_effect(&D_004A9160);
                    if(effect) { EFFECT(effect,&obj->xEC); }
                }
                return;
            }
            {
                float result;
                st.u.v[2]=obj->xDC;
                result=func_00203C28(&st.u.v[2]);
                if(result>=0.99f || result<=0.0f) {
                    effect=alloc_effect(&D_004A9160);
                    if(effect) { EFFECT(effect,&obj->xDC); }
                }
            }
        } else func_0012F328(rider,contact,D_0029C190,1.0f);
        break;
    case 0x4BF: case 0x4C1: case 0x4C3: case 0x4C6:
        func_0012F328(rider,contact,D_0029C194,1.0f);
        obj->x268 |= 1UL<<48;
        break;
    default:
        model=obj->x10;
        COPY_VECTOR(st.u.v[0],&model->x50);
        pos=&obj->x170;
        COPY_VECTOR(st.u.v[1],pos);
        phys=get_physics(rider);
        tmp=&st.u.v[1];
        SUB(st.u.v[2],tmp,&rider->x4180);
        one=1.0f;
        factor=dot_vec(&st.u.v[2],&contact->x20) * -2.0f / (dot_vec(&contact->x20,&contact->x20) * (one / phys->mass + one));
        MADD(*tmp,tmp,&contact->x20,factor);
        length=normalize_length(&model->x50,tmp);
        *(U128 *)&st.v30=0;
        st.v30.z=1.0f;
        CROSS(st.v50,&obj->x10->x50,&st.v30);
        normalize(&obj->x10->x40,&st.v50);
        CROSS(obj->x10->x60,&obj->x10->x40,&obj->x10->x50);
        SCALE(rider->x4120,&rider->x4120,0.9f);
        obj->x10->x50.x=func_00235EA8(st.u.v[0].x,obj->x10->x50.x,0.06f,0.001f);
        obj->x10->x50.y=func_00235EA8(st.u.v[0].y,obj->x10->x50.y,0.06f,0.001f);
        obj->x10->x50.z=func_00235EA8(st.u.v[0].z,obj->x10->x50.z,0.06f,0.001f);
        SCALE(st.v60,&obj->x10->x50,length);
        w=pos->w;
        st.v60.z=0.0f;
        COPY_VECTOR(obj->x170,&st.v60);
        pos->w=w;
        break;
    }
}
