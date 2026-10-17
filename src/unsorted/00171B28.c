/* UNIT 0x00171B28 0x00171DD0 */
typedef struct { float x,y,z,w; } Vec;
extern float D_0029C8C8[];
extern int D_004B2828[];
typedef struct { char pad0[0x50130]; char *nodes[1]; } Global;
typedef union { Global *ptr; unsigned int value; } Addr;
extern Global D_002AF2C0;
extern float func_00235EA8(float,float,float,float);
extern void func_00171958(void *,int),func_001DD668(void *,Vec *,void *),func_001DCE58(void *),func_001DD030(void *,void *,int,int),func_001DD218(void *,Vec *,void *,float,float),func_001722A0(void *,float);
#define F(p,n) (*(float *)((char *)(p)+(n)))
#define I(p,n) (*(int *)((char *)(p)+(n)))
#define P(p,n) (*(char **)((char *)(p)+(n)))
#define B(p,n) (*(unsigned char *)((char *)(p)+(n)))
#define H(p,n) (*(unsigned short *)((char *)(p)+(n)))
#define COPY_VECTOR(dst, src) \
    __asm__ volatile ("lq $2, 0x0(%1)\n\tsq $2, %0" : "=m"(dst) : "r"(src) : "$2")
/* VU0 scale-add: d = a + b * k (xyz). */
static inline void MADD(Vec *d,void *a,void *b,float k) { __asm__ volatile("lqc2 $vf11,0(%1)\n\tlqc2 $vf12,0(%2)\n\tmfc1 $2,%3\n\tqmtc2.ni $2,$vf13\n\tvaddax.xyz ACC,$vf11,$vf0x\n\tvmaddx.xyz $vf11,$vf12,$vf13x\n\tsqc2 $vf11,%0":"=m"(*d):"r"(a),"r"(b),"f"(k):"$2"); }
static inline float Abs(float x) { float r; __asm__ volatile ("abs.s %0,%1" : "=&f"(r) : "f"(x)); return r; }
/* The int parameter keeps the stored 1 in its own SImode pseudo. */
static inline void SetTarget(char *state,float speed,int active) { F(state,0x30)=speed; B(state,0x3C)=active; I(state,0x34)=0; }
void func_00171B28(char *obj) {
    Vec result __attribute__((aligned(16))),temp __attribute__((aligned(16)));
    if(B(obj,0x209)) {
        char *state=obj+0x1A0;
        char *move=obj+0xA0;
        func_001DD668(move,&result,state);
        if(F(state,0x38)<2.0f || F(move,0x58)*0.6818182f<1.0f) func_00171958(obj,1);
    } else {
        char *path=obj+0x110;
        char *point=P(path,8);
        if((H(point,0x14)>>1)&1) {
            char *state=obj+0x1A0;
            char *move;
            COPY_VECTOR(*(Vec *)(obj+0x1A0),point);
            F(state,0x38)=D_0029C8C8[0];
            MADD(&temp,P(path,8),obj+0x140,-17.0f);
            COPY_VECTOR(*(Vec *)(obj+0x1B0),&temp);
            move=obj+0xA0;
            SetTarget(state,F(move,0x44)*2.0777779f,1);
            func_001DD668(move,&result,state);
            if(F(move,0x58)*0.6818182f>1.0f) B(obj,0x209)=1;
        } else {
            if(B(P(obj,8),0x29CC)) {
                int count=D_004B2828[0];
                float nearest=D_0029C8C8[0];
                if(count>0) {
                    Addr address;
                    char **nodes;
                    int remaining;
                    float original=F(P(obj,4),0x18);
                    address.ptr=&D_002AF2C0;
                    nodes=(char **)(address.value+0x50130);
                    remaining=count;
                    do {
                        float delta=F(P(*nodes,0x792C),0x18)-original;
                        if(Abs(delta)<Abs(nearest)) nearest=delta;
                        --remaining; ++nodes;
                    } while(remaining);
                }
                func_001722A0(obj,F(P(obj,8),0x2994));
            }
            if(F(obj,0x1F0)<F(P(obj,8),0x2998)) {
                if(B(P(obj,8),0x29CC)) F(obj,0x1F0)=F(P(obj,8),0x2998);
                else F(obj,0x1F0)=func_00235EA8(F(obj,0x1F0),F(P(obj,8),0x2998),0.03f,0.001f);
            }
            func_001DD218(obj+0xA0,&result,obj+0x110,F(obj,0x1F0),F(obj,0x1F4));
        }
    }
    COPY_VECTOR(*(Vec *)(obj+0xD0),&result);
    func_001DCE58(obj+0xA0);
    func_001DD030(obj+0xA0,P(obj,4)+0x40,0,1);
}
