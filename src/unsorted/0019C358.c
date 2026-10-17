/* UNIT 0x0019C358 0x0019C670 .sdata=0x29D1B4:0x29D1B8 */
typedef struct Heap { int x0, x4, x8; } Heap;
typedef struct Obj { int x0; unsigned short *x4; } Obj;
typedef struct Weights { float x[3][2]; } Weights;
extern Heap D_00663B08;
extern Weights D_0028E5B8;
int D_0029D1B4 = 112;
extern void func_00218A30(void);
extern void func_0019C220(Obj *, int, void *, unsigned short, unsigned short, int, int);
extern float func_0019C2F8(unsigned int *, int, int, int, float);

static __inline__ void push(Heap *h) { int old = h->x8; Heap *p = (Heap *)((h->x4 + 3) & ~3); h->x8 = (int)p; h->x4 = (int)p + 4; p->x0 = old; }
static __inline__ void *alloc(Heap *h, int n) { int p = (h->x4 + 127) & ~127; h->x4 = p + n; return (void *)p; }

/* Builds a 384x224 RGB555 image in o->x4 by sampling two pages of 112 rows,
   blending 4 source columns into 3 with the weights in D_0028E5B8. */
void func_0019C358(Obj *const o) {
 Weights weights;
 unsigned int *pixels;
 int page;
 int x;
 int y;
 float sum;
 unsigned short *dst;
 func_00218A30();
 if (!o->x4) { push(&D_00663B08); o->x4 = alloc(&D_00663B08, 0x2a000); }
 push(&D_00663B08);
 pixels = alloc(&D_00663B08, 0x38000);
 weights = D_0028E5B8;
 for (page = 0; page < 2; page++) {
  func_0019C220(o, 0x700, pixels, 0, page * D_0029D1B4, 512, D_0029D1B4);
  for (x = 0; x < 128; x++) {
   for (y = 0; y < 112; y++) {
    int part;
    for (part = 0; part < 3; part++) {
     int channel;
     dst = o->x4 + (x * 3 + part + (y + page * 112) * 384);
     *dst = 0;
     for (channel = 0; channel < 3; channel++) {
      sum = func_0019C2F8(pixels, x * 4 + part, y, channel * 8, weights.x[part][0]);
      sum += func_0019C2F8(pixels, x * 4 + part + 1, y, channel * 8, weights.x[part][1]);
      *dst |= (unsigned short)(int)(sum * 0.12549f) << channel * 5;
     }
    }
   }
  }
 }
 D_00663B08.x4 = D_00663B08.x8;
 D_00663B08.x8 = *(int *)D_00663B08.x8;
}
