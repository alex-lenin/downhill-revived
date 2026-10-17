/* UNIT 0x0016CE68 0x0016D0C0 */
struct Vec { float x,y,z,w; };
struct Vtable { char pad[32]; short delta,pad22; void *(*method)(void *); };
struct Node { struct Vtable *vt; char pad[0x22c]; void *x230; };
struct Part { char pad[0x130]; struct Node *x130; char tail[0xfc]; };
struct Obj { int x0; char pad4[0x1ec]; struct Vec x1f0; char pad200[0x50]; struct Part x250,x480; int x6b0; };
extern char D_00412510[],D_00308D40[];
extern void func_0016EFF8(void *,int,void *,void *,int,int,int,int);
extern void *func_00164170(void *,void *,void *);
extern int func_001272E0(void *,void *),func_00163D70(void *,void *);
extern struct Node *func_001930C8(int,void *);
extern void func_001224F0(void *,void *,void *,int,int),func_0016F9F8(void *),func_0016E8D8(void *,void *,void *);
static inline char *getPos(struct Part *part) {
 struct Node *node=part->x130;
 if(node) return (char *)node->vt->method((char *)node+node->vt->delta)+16;
 return 0;
}
static inline void normalize(struct Vec *v) {
 float x=v->x,y=v->y,z=v->z,len,inv;
 __asm__ volatile("mula.s %1,%1\n\tmadda.s %2,%2\n\tmadd.s %0,%3,%3":"=f"(len):"f"(x),"f"(y),"f"(z));
 __asm__ volatile("rsqrt.s %0,%1,%2":"=&f"(inv):"f"(1.0f),"f"(len));
 v->x=x*inv;v->y=y*inv;v->z=z*inv;
}
void func_0016CE68(struct Obj *obj, int a1, void *a2, int a3, int a4, int a5, int a6) {
    struct Vec vec;
    void *id;
    struct Node *node;
    char *data;
    char *sub;

    obj->x6b0 = a1;
    func_0016EFF8((char *)obj + 16, a1, obj, a2, a3, a4, a5, a6);
    obj->x0 = 0;
    if (a2) {
        sub = (char *)a2 + 16;
        id = func_00164170(D_00412510, sub, 0);
        if (id) {
            node = func_001930C8(10, id);
            if (node) {
                a2 = &obj->x250;
                {
                    int idx, n;
                    unsigned int offset;
                    idx = func_001272E0(&obj->x250, id);
                    offset = idx * 240 + 16;
                    data = D_00308D40 + offset;
                    n = func_00163D70(D_00412510, id);
                    func_001224F0(a2, node, data, n, 10);
                }
                node->x230 = a2;
                id = func_00164170(D_00412510, sub, id);
                if (id) {
                    node = func_001930C8(10, id);
                    if (node) {
                        int n = func_00163D70(D_00412510, id);
                        do {
                            a2 = &obj->x480;
                            func_001224F0(a2, node, data, n, 10);
                        } while (0);
                        node->x230 = a2;
                        obj->x0 = a6;
                    }
                }
            }
        }
    }
    func_0016F9F8((char *)obj + 16);
    switch (obj->x0) {
    case 2: case 6: case 7: {
        struct Vec *pos = &obj->x1f0;
        struct Part *which;
        __asm__ volatile("lqc2 $vf11,0(%1)
	lqc2 $vf12,0(%2)
	vsub.xyz $vf11,$vf11,$vf12
	sqc2 $vf11,%0"
            : "=m"(vec) : "r"(pos), "r"(getPos(&obj->x250)));
        normalize(&vec);
        which = &obj->x250;
        func_0016E8D8(obj, which, &vec);
        func_0016E8D8(obj, &obj->x480, &vec);
        }
        break;
    }
}
