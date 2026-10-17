/* UNIT 0x0019F210 0x0019F9B0 */
typedef struct Vec { float x, y, z, w; } Vec;
typedef struct Track { char pad[0x18]; float x18; } Track;
typedef struct Rider {
    char pad0[0x6280];
    char x6280[0x6610 - 0x6280];
    char x6610[0xC];
    float x661C;
    char pad6620[0x792C - 0x6620];
    Track *x792C;
    char pad7930[0x797C - 0x7930];
    int x797C;
    char pad7980[0x7988 - 0x7980];
    unsigned int x7988;
    int x798C;
    int x7990;
    char pad7994[0x79B0 - 0x7994];
    int x79B0[43];
    int x7A5C;
    char pad7A60[0x7BD0 - 0x7A60];
} Rider;
typedef struct Game {
    Rider riders[10];
    char pad4D620[0x50130 - 0x4D620];
    Rider *table[30];
    int count;
} Game;
typedef struct State {
    char pad0[0x10C];
    int x10C;
    char pad110[0x288 - 0x110];
    int x288;
    char pad28C[0x358 - 0x28C];
    int x358;
} State;
typedef struct Other {
    char pad0[0x130];
    Rider *table[30];
    int x1A8;
} Other;
typedef struct Scores {
    char pad0[0x10];
    int x10;
    char pad14[0x1C - 0x14];
    int x1C[2];
} Scores;
typedef struct Obj {
    Rider *x0;
    Rider *x4;
    int x8;
    char padC[0x18 - 0xC];
    int x18;
    int x1C[10];
    char pad44[0x50 - 0x44];
    int x50;
    char pad54[0x58 - 0x54];
    unsigned char x58[10];
    unsigned char x62[10];
    char pad6C[0x220 - 0x6C];
    Vec x220[8];
    char pad2A0[0x460 - 0x2A0];
    unsigned char x460[36];
    int x484;
} Obj;

extern Game D_002AF2C0;
extern Other D_002FF2C0;
extern State D_004B25A0;
extern Scores D_0038DB60;
extern char D_003B2270[];
extern char D_00398C40[];
extern int D_0043F9B0[];

extern void func_0019F9B0(Obj *obj, Rider *r, int idx);
extern float func_00221C80(float *a, float *b);
extern void func_001E6420(void *p, float f);
extern void func_0017C010(void *p, int v);
extern void func_001D4890(void *p);
extern void func_0012FE38(void *a, void *b);

static __inline__ Rider *riderAt(int i) { Rider *base = D_002AF2C0.riders; return (Rider *)((char *)base + i * 0x7BD0); }
static __inline__ void notify(char *base, int c, unsigned int u) { if (u < 4) func_0017C010(base + (u * 0x1380 + 0x150), c); }
static __inline__ void sound(void) { char *p = D_00398C40; func_001D4890(p + 0x240); }
void func_0019F210(Obj *obj)
{
    Rider *r;
    int used[12];
    Rider *pending[12];
    int scores[2];
    int npend;
    int best;
    int count;
    int i;
    int rank;
    int t;
    npend = 0;
    best = 0;
    count = D_002AF2C0.count;

    if (obj->x50 != 0) {
        int *xp;
        Rider **pp;
        int one;
        int type;
        rank = 2;
        D_0043F9B0[0] = 0;
        i = 0;
        if (count > 0) {
            r = &D_002AF2C0.riders[0];
            one = 1;
            pp = &pending[0];
            xp = &obj->x1C[0];
            do {
                type = r->x7A5C;
                if (type == one) {
                    *pp = r;
                    *xp = obj->x8;
                    r->x798C = type;
                } else {
                    *pp = r;
                    *xp = obj->x8 + rank + 1;
                    r->x798C = rank;
                    rank++;
                }
                func_0019F9B0(obj, r, i);
                i++;
                xp++;
                r++;
            } while (i < count);
        }
        obj->x50 = 0;
        return;
    }
    if (obj->x484 > 0) {
        for (i = 0; i < count; i++) {
            r = riderAt(i);
            if (r->x7A5C == 0)
                continue;
            if (obj->x58[i] != 0)
                continue;
            {
                int done = 0;
                if (obj->x62[i] != 0) {
                    obj->x1C[i] = obj->x8;
                    pending[npend] = r;
                    npend++;
                    obj->x62[i] = 0;
                    done = 1;
                }
                if (!done) {
                    int c = r->x7990;
                    float d;
                    d = func_00221C80((float *)&obj->x220[c + 1], (float *)((char *)r->x792C + 0x10));
                    if (d > 0.0f) {
                        r->x7990 = c + 1;
                        r->x79B0[c] = obj->x8;
                        if (obj->x18 != 0 && obj->x460[c] != 0 && r->x7A5C == 1) {
                            int mode = D_004B25A0.x10C;
                            if (mode == 13 || mode == 16 || mode == 14 || mode == 18)
                                continue;
                            if (D_004B25A0.x358 <= 0)
                                continue;
                            switch (r->x798C) {
                            case 1:
                                func_001E6420(r->x6610, 200.0f);
                                break;
                            case 2:
                                func_001E6420(r->x6610, 100.0f);
                                break;
                            case 3:
                                func_001E6420(r->x6610, 50.0f);
                                break;
                            default:
                                break;
                            }
                            {
                                unsigned int u = r->x7988;
                                notify(D_003B2270, c, u);
                                sound();
                            }
                        }
                    } else if (c != 0) {
                        float e = func_00221C80((float *)&obj->x220[c], (float *)((char *)r->x792C + 0x10));
                        float f = e < 0.0f ? 1.0f : 0.0f;
                        if (f != 0.0f) {
                            r->x7990 = c - 1;
                            r->x79B0[c] = 0;
                        }
                    }
                }
            }
        }
    }
    if (count > 0) {
        int *p = &used[0];
        i = count;
        do {
            *p++ = 0;
        } while (--i != 0);
    }
    if (D_004B25A0.x10C == 13) {
        i = 0;
        if (D_004B25A0.x288 > 0) {
            do {
                int top = -1;
                for (t = 0; t < D_004B25A0.x288; t++) {
                    if (used[t] == 0) {
                        int v;
                        r = D_002AF2C0.table[t];
                        v = (int)r->x661C;
                        if (top < v) {
                            top = v;
                            best = t;
                        }
                    }
                }
                if (top != -1) {
                    r = D_002AF2C0.table[best];
                    used[best] = 1;
                    r->x798C = i + 1;
                }
                i++;
            } while (i < D_004B25A0.x288);
        }
    } else if (D_004B25A0.x10C == 16) {
        Rider *a = D_002AF2C0.table[0];
        Rider *b = D_002AF2C0.table[1];
        if (obj->x58[0] != 0 && obj->x58[1] != 0) {
            scores[0] = obj->x1C[0] + (D_0038DB60.x10 - D_0038DB60.x1C[0]) * 60;
            scores[1] = obj->x1C[1] + (D_0038DB60.x10 - D_0038DB60.x1C[1]) * 60;
            if (scores[0] < scores[1]) {
                obj->x0 = a;
                a->x798C = 1;
                b->x798C = 2;
            } else {
                obj->x0 = b;
                a->x798C = 2;
                b->x798C = 1;
            }
        } else if (a->x792C->x18 < b->x792C->x18) {
            obj->x0 = a;
            a->x798C = 1;
            b->x798C = 2;
        } else {
            obj->x0 = b;
            a->x798C = 2;
            b->x798C = 1;
        }
    } else {
        int placed = 0;
        for (i = 0; i < count;) {
            int low = 0x7FFFFFFF;
            for (t = 0; t < count; t++) {
                r = riderAt(t);
                if (r->x7A5C == 0)
                    continue;
                if (used[t] != 0)
                    continue;
                {
                    int found = 0;
                    int q;
                    {
                        for (q = 0; q < npend; q++) {
                            if (pending[q] == r)
                                found = 1;
                        }
                    }
                    if (obj->x58[t] == 0 && found == 0)
                        continue;
                    if (obj->x1C[t] < low) {
                        low = obj->x1C[t];
                        best = t;
                    }
                }
            }
            if (low != 0x7FFFFFFF) {
                r = riderAt(best);
                r->x798C = i + 1;
                if (i == 0)
                    obj->x0 = r;
                else if (i == D_002AF2C0.count - 1)
                    obj->x4 = r;
                placed++;
                used[best] = 1;
            }
            i++;
        }
        {
            for (t = 0; t < count; t++) {
                int j;
                float lowf;
                best = -1;
                lowf = 1.6e10f;

                for (j = 0; j < count; j++) {
                    r = riderAt(j);
                    if (r->x7A5C == 0)
                        continue;
                    if (used[j] != 0)
                        continue;
                    {
                        float z = r->x792C->x18;
                        if (z < lowf) {
                            lowf = z;
                            best = j;
                        }
                    }
                }
                if (best >= 0) {
                    used[best] = 1;
                    r = riderAt(best);
                    r->x798C = placed + t + 1;
                    if (t == 0 && placed == 0)
                        obj->x0 = r;
                    else if (t == D_002AF2C0.count - 1)
                        obj->x4 = r;
                }
            }
        }
    }
    if (npend > 0) {
        Rider **pp = &pending[0];
        i = npend;
        do {
            Rider *p1 = *pp;
            Rider *p2;
            i--;
            func_0019F9B0(obj, p1, p1->x797C);
            p2 = *pp;
            pp++;
            func_0012FE38(p2, p2->x6280);
        } while (i != 0);
    }
}
