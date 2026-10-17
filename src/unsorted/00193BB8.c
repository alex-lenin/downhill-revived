/* UNIT 0x00193BB8 0x00193E30 */
typedef struct Big {
    char pad[0x694];
} Big;

typedef struct Mid1 {
    char pad[0xD8];
} Mid1;

typedef struct Mid2 {
    char pad[0x420];
} Mid2;

typedef struct Small {
    char pad[0x28];
} Small;

typedef struct Elem50 {
    char pad[0x50];
} Elem50;

typedef struct Obj {
    Big big[18];            /* 0x0000 */
    Mid1 mid1[33];          /* 0x7668 */
    Mid2 mid2[33];          /* 0x9240 */
    Small small;            /* 0x11A60 */
    Elem50 elems[6];        /* 0x11A88 */
    char pad11C68[0x4A8];   /* 0x11C68 */
    int x12110;             /* 0x12110: set by func_00193E30 */
    int x12114;             /* 0x12114 */
    long x12118[2];         /* 0x12118 */
    int x12128;             /* 0x12128 */
    int x1212C;             /* 0x1212C */
    long x12130[5];         /* 0x12130 */
    int x12158[5];          /* 0x12158 */
    int x1216C[5];          /* 0x1216C */
    int x12180;             /* 0x12180 */
    int x12184;             /* 0x12184 */
    int x12188;             /* 0x12188 */
    char gap1218C[4];       /* 0x1218C: untouched */
    char x12190[23];        /* 0x12190 */
    char x121A7[4][12];     /* 0x121A7 */
    char x121D7[4][12];     /* 0x121D7 */
    char gap12207;          /* 0x12207: untouched */
    int x12208;             /* 0x12208 */
    int x1220C;             /* 0x1220C */
    int x12210;             /* 0x12210 */
    int x12214;             /* 0x12214 */
    short x12218;           /* 0x12218 */
    short x1221A;           /* 0x1221A */
    short x1221C;           /* 0x1221C */
    short x1221E;           /* 0x1221E */
    int x12220[4];          /* 0x12220 */
} Obj;

extern void func_00195040(void *p);
extern void func_00195480(void *p);
extern void func_00195B00(void *p);
extern void func_001951B0(void *p);
extern void func_00195BB8(void *p);
extern void func_00193E30(void *p);

void func_00193BB8(Obj *obj)
{
    int i;
    int j;
    int k;
    Mid1 *p1;
    Mid2 *p2;
    Elem50 *pe;
    char *q;
    int *qw;

    for (i = 0; i < 18; i++)
        func_00195040(&obj->big[i]);

    p1 = obj->mid1;
    p2 = obj->mid2;
    for (i = 0; i < 33; i++) {
        func_00195480(p1);
        p1++;
        func_00195B00(p2);
        p2++;
    }

    pe = obj->elems;
    for (i = 0; i < 6; i++) {
        func_001951B0(pe);
        pe++;
    }

    func_00195BB8(&obj->small);
    obj->x12210 = 0x0FFFFFFF;
    obj->x12214 = 0;
    func_00193E30(obj);

    q = &obj->x12190[22];
    for (i = 0; i < 23; i++) {
        *q = 0;
        q--;
    }

    obj->x12114 = 0x10020C3;
    obj->x12118[1] = 0;
    obj->x12118[0] = 0;
    obj->x12128 = 0;
    obj->x1212C = 0;
    obj->x12180 = 0;
    obj->x12184 = 0;
    obj->x12188 = 0;

    for (k = 0; k < 5; k++) {
        obj->x12130[k] = 0;
        obj->x12158[k] = 0;
        obj->x1216C[k] = 0;
    }

    obj->x12208 = 0;
    obj->x1220C = 0;

    for (k = 0; k < 4; k++)
        for (j = 11; j >= 0; j--)
            obj->x121A7[k][j] = 0x21;

    for (k = 0; k < 4; k++)
        for (j = 11; j >= 0; j--)
            obj->x121D7[k][j] = 0x28;

    obj->x12218 = 0xE0;
    obj->x1221A = 0x180;
    obj->x1221C = 0x1A0;
    obj->x1221E = 0x140;

    qw = &obj->x12220[3];
    for (k = 0; k < 4; k++) {
        *qw = 2;
        qw--;
    }
}
