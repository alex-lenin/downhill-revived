/* UNIT 0x001AAFC0 0x001AB0D0 */
typedef struct Vec {
    float x, y, z, w;
} Vec __attribute__((aligned(16)));

typedef struct Item {
    char pad0[0x8];
    float scale;
    char padC[0x10];
    float x1C;
    Vec x20;
} Item;

typedef struct Group {
    char pad0[0x20];
    Item *items;
    int count;
} Group;

typedef struct Model {
    char pad0[0x1C];
    int count;
    char pad20[0x20];
    char x40[1];
} Model;

typedef struct Obj {
    char pad0[0x20];
    Model *model;
} Obj;

extern void func_00231A48(void *dst, Vec *v);
extern float fabsf(float);

void func_001AAFC0(Obj *obj, Group *groups)
{
    Vec max;
    Vec tmp;
    int i;
    int n;
    Item *item;

    __asm__ volatile ("sqc2 $vf0, %0" : "=m"(max));
    for (i = 0; i < obj->model->count; i++) {
        n = groups[i].count;
        item = groups[i].items;
        while (n != 0) {
            if (item->x1C < 0.0f) {
                __asm__ volatile ("lqc2 $vf11, 0x0(%1)\n\t"
                                  "mfc1 $2, %2\n\t"
                                  "qmtc2.ni $2, $vf12\n\t"
                                  "vmulx.xyz $vf11, $vf11, $vf12x\n\t"
                                  "sqc2 $vf11, %0"
                                  : "=m"(tmp) : "r"(&item->x20), "f"(item->scale) : "$2");
                { float s,a,m,ma;
                  s=tmp.x; a=fabsf(s);
                  __asm__ volatile("Lxx_a:");
                  m=max.x; ma=fabsf(m); __asm__ volatile("Lxx_m:" : : "f"(m));
                  if(ma<a) max.x=s; }
                { float s,a,m,ma;
                  s=tmp.y; a=fabsf(s);
                  __asm__ volatile("Lyy_a:");
                  m=max.y; ma=fabsf(m); __asm__ volatile("Lyy_m:" : : "f"(m));
                  if(ma<a) max.y=s; }
                { float s,a,m,ma;
                  s=tmp.z; a=fabsf(s);
                  __asm__ volatile("Lzz_a:");
                  m=max.z; ma=fabsf(m); __asm__ volatile("Lzz_m:" : : "f"(m));
                  if(ma<a) max.z=s; }
            }
            n--;
            item++;
        }
    }
    func_00231A48(obj->model->x40, &max);
}
