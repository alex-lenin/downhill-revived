/* UNIT 0x001403C8 0x00140450 */
typedef struct Entry { short delta; short pad; void (*fn)(void *); } Entry;
typedef struct Obj { char pad[0x4F8]; Entry *x4F8; } Obj;
extern int D_004B26AC[];
typedef struct State { char pad[0x1218C]; char flags[3]; } State;
extern State D_004225A0;
extern char D_004C8508[];
extern void func_001F9CF8(void *);
extern void func_0013A578(void);
static __inline__ int limit(int i,int n) { return i>n ? n+1 : i; }
static __inline__ void clear(State *p,int i) { p->flags[i]=0; }
void func_001403C8(Obj *p) {
 int i=limit(D_004B26AC[0],1);
 clear(&D_004225A0,i);
 func_001F9CF8(D_004C8508+i*0xA0);
 func_0013A578();
 p->x4F8[8].fn((char *)p+p->x4F8[8].delta);
}
