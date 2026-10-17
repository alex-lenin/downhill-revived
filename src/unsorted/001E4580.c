/* UNIT 0x001E4580 0x001E47B8 */
typedef struct Obj {
    char pad0[0x500];
    int x500;
    char pad504[0x14];
    unsigned char x518, x519, x51A, x51B, x51C, x51D, x51E, x51F;
} Obj;

typedef struct Ent { int x0; char pad4[8]; } Ent;

void func_001E4580(Obj *obj, Ent *arr, int n, int a4, unsigned char flag, int a6)
{
    int i;
    obj->x51F = flag;
    switch (obj->x500) {
    case 6:
        if (n >= 2) {
            i = 0;
            while (i < n && arr[i].x0 != 0x15) i++;
            while (i < n && arr[i].x0 != 0xD) i++;
            if (i < n) obj->x518 = 1;
        }
        break;
    case 7:
    case 10:
        if (n >= 3) {
            i = 0;
            while (i < n && arr[i].x0 != 0x14) i++;
            while (i < n && arr[i].x0 != 7) i++;
            while (i < n && arr[i].x0 != 0x1C) i++;
            while (i < n && arr[i].x0 != 0x1B) i++;
            if (obj->x500 == 0xA) {
                if (i < n) obj->x51B = 1;
            } else {
                if (i < n) obj->x518 = 1;
            }
        }
        break;
    }
}
