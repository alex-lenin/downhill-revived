/* UNIT 0x00143F10 0x00144148 */
typedef struct Item { char pad0[0x110]; } Item;

typedef struct Obj {
    Item items[7];
    int x770[12];
    int count;
} Obj;

extern char D_0036CFC0[];
extern int func_00163B70(void *);
extern int func_00163BE0(void *, int);
extern void *func_00163D40(void *, int);
extern int func_00222BD8(int, int);
extern void func_00144248(Item *, Obj *, int, int, void *);

void func_00143F10(Obj *obj)
{
    int n1 = func_00163B70(D_0036CFC0);
    int count = obj->count;
    int j;

    if (count != 0 && n1 > 0) {
        int i = 0;
        int handle = -1;
        int groups = n1;
        int remaining = count;

        for (j = 0; j < n1; j++) {
            int idx;
            int rem;
            int q;
            int extra;
            int chunk;
            int k;
            handle = func_00163BE0(D_0036CFC0, handle);
            idx = handle;
            rem = remaining % groups;
            q = remaining / groups;
            extra = rem != 0;
            chunk = extra + q;
            for (k = 0; k < chunk && i < obj->count; k++) {
                void *val = func_00163D40(D_0036CFC0, idx);
                func_00144248(&obj->items[i], obj, obj->x770[i], i, val);
                idx += func_00222BD8(2, 3);
                i++;
                remaining--;
            }
            if (groups >= 2)
                groups--;
        }
    } else {
        int idx = func_00222BD8(0, 2);
        int i;
        for (i = 0; i < obj->count; i++) {
            void *val;
            idx += func_00222BD8(2, 3);
            val = func_00163D40(D_0036CFC0, idx);
            func_00144248(&obj->items[i], obj, obj->x770[i], i, val);
        }
    }
}
