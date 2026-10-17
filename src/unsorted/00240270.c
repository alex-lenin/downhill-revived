/* UNIT 0x00240270 0x002421C0 .sdata=0x29DCC8:0x29DCE8 */
#include "include_asm.h"
int D_0029DCC8=0;
int D_0029DCCC=0, D_0029DCD0=0;
unsigned int D_0029DCD4=0;
long D_0029DCD8 = 0;
char *D_0029DCE0 = 0;
int D_0029DCE4 = 0;
int D_0029DCE8[0] __attribute__((weak, section(".sdata"))) = {};
extern long D_0029E470;
extern char *D_0029E48C;
extern unsigned char *D_0029E488; /* read cursor */
extern unsigned char *D_0029E490; /* start of buffer */
extern int D_0029E494;            /* nonzero: stream from memory buffer */
extern long func_00240080(long, char *, long);
int func_00240270(long count) {
 D_0029DCD8 = func_00240080(D_0029DCD8, D_0029E48C, count);
 D_0029E48C += (int)count;
 D_0029E470 -= count;
 return 0;
}

int func_002402E0(void) {
    unsigned char *size;
    unsigned short *nameLength;
    int value;

    D_0029E488 = (unsigned char *)0x01FDF800;
    if (*(int *)D_0029E488 != 0x04034549) {
        return 0;
    }
    size = (unsigned char *)0x01FDF816;
    nameLength = (unsigned short *)0x01FDF81A;
    value = size[0] + (size[1] << 8) + (size[2] << 16) + (size[3] << 24);
    D_0029E488 = (unsigned char *)nameLength + 4 + *nameLength;
    return value;
}

typedef struct Huft {
    unsigned char e;
    unsigned char b;
    union {
        unsigned short n;
        struct Huft *t;
    } v;
} Huft;
extern const unsigned short D_00299440[];
extern unsigned char *D_0029E468;
extern void *memcpy(void *, const void *, unsigned int);

extern unsigned char func_00242058(void);
int func_00240358(Huft *tl, Huft *td, int bl, int bd)
{
    register unsigned e;
    unsigned n, d;
    unsigned w;
    Huft *t;
    unsigned ml, md;
    register unsigned long b;
    register unsigned k;
    int r = 0;

    ml = D_00299440[bl];
    md = D_00299440[bd];
    b = D_0029DCD4;
    k = D_0029DCD0;
    w = D_0029DCCC;
    for (;;) {
        while (k < (unsigned)bl) {
            b |= (unsigned long)(func_00242058() & 255) << k;
            k += 8;
        }
        if ((e = (t = tl + ((unsigned)b & ml))->e) > 16) {
            do {
                if (e == 99)
                    return 1;
                k -= t->b;
                b >>= t->b;
                e -= 16;
                while (k < e) {
                    b |= (unsigned long)(func_00242058() & 255) << k;
                    k += 8;
                }
            } while ((e = (t = t->v.t + ((unsigned)b & D_00299440[e]))->e) > 16);
        }
        b >>= t->b;
        k -= t->b;
        if (e == 16) {
            D_0029E48C[w++] = (unsigned char)t->v.n;
            if (w == 0x20000) {
                r = func_00240270(0x20000);
                if (r)
                    return r;
                w = 0;
            }
        } else {
            if (e == 15)
                break;
            while (k < e) {
                b |= (unsigned long)(func_00242058() & 255) << k;
                k += 8;
            }
            n = t->v.n + ((unsigned)b & D_00299440[e]);
            k -= e;
            b >>= e;
            while (k < (unsigned)bd) {
                b |= (unsigned long)(func_00242058() & 255) << k;
                k += 8;
            }
            if ((e = (t = td + ((unsigned)b & md))->e) > 16) {
                do {
                    if (e == 99)
                        return 1;
                    k -= t->b;
                    b >>= t->b;
                    e -= 16;
                    while (k < e) {
                        b |= (unsigned long)(func_00242058() & 255) << k;
                        k += 8;
                    }
                } while ((e = (t = t->v.t + ((unsigned)b & D_00299440[e]))->e) > 16);
            }
            k -= t->b;
            b >>= t->b;
            while (k < e) {
                b |= (unsigned long)(func_00242058() & 255) << k;
                k += 8;
            }
            d = w - t->v.n - ((unsigned)b & D_00299440[e]);
            k -= e;
            b >>= e;
            do {
                d &= 0x1ffff;
                if (d < w)
                    D_0029E468 = (unsigned char *)D_0029E48C;
                else
                    D_0029E468 = (unsigned char *)D_0029E48C - 0x20000;
                n -= (e = (e = 0x20000 - ((d > w) ? d : w)) > n ? n : e);
                if ((unsigned)(w - d) >= e) {
                    memcpy(D_0029E48C + w, D_0029E468 + d, e);
                    w += e;
                    d += e;
                } else {
                    do {
                        D_0029E48C[w++] = D_0029E468[d++];
                    } while (--e);
                }
                if (w == 0x20000) {
                    r = func_00240270(0x20000);
                    if (r)
                        return r;
                    w = 0;
                }
            } while (n != 0);
        }
    }

    D_0029DCCC = w;
    D_0029DCD4 = (unsigned int)b;
    D_0029DCD0 = k;
    return r;
}


int func_002407F8(tl, td, bl, bd)
Huft *tl, *td;
int bl, bd;
{
    register unsigned e;
    unsigned n, d;
    unsigned w;
    Huft *t;
    unsigned ml, md;
    register unsigned long b;
    register unsigned k;
    unsigned char *in;
    int r = 0;

    ml = D_00299440[bl];
    md = D_00299440[bd];
    b = D_0029DCD4;
    k = D_0029DCD0;
    w = D_0029DCCC;
    in = D_0029E488;
    for (;;) {
        while (k < (unsigned)bl) {
            b |= (unsigned long)*in++ << k;
            k += 8;
        }
        if ((e = (t = tl + ((unsigned)b & ml))->e) > 16) {
            do {
                if (e == 99)
                    return 1;
                k -= t->b;
                b >>= t->b;
                e -= 16;
                while (k < e) {
                    b |= (unsigned long)*in++ << k;
                    k += 8;
                }
            } while ((e = (t = t->v.t + ((unsigned)b & D_00299440[e]))->e) > 16);
        }
        b >>= t->b;
        k -= t->b;
        if (e == 16) {
            D_0029E48C[w++] = (unsigned char)t->v.n;
            if (w == 0x20000) {
                r = func_00240270(0x20000);
                if (r)
                    return r;
                w = 0;
            }
        } else {
            if (e == 15)
                break;
            while (k < e) {
                b |= (unsigned long)*in++ << k;
                k += 8;
            }
            n = t->v.n + ((unsigned)b & D_00299440[e]);
            k -= e;
            b >>= e;
            while (k < (unsigned)bd) {
                b |= (unsigned long)*in++ << k;
                k += 8;
            }
            if ((e = (t = td + ((unsigned)b & md))->e) > 16) {
                do {
                    if (e == 99)
                        return 1;
                    k -= t->b;
                    b >>= t->b;
                    e -= 16;
                    while (k < e) {
                        b |= (unsigned long)*in++ << k;
                        k += 8;
                    }
                } while ((e = (t = t->v.t + ((unsigned)b & D_00299440[e]))->e) > 16);
            }
            k -= t->b;
            b >>= t->b;
            while (k < e) {
                b |= (unsigned long)*in++ << k;
                k += 8;
            }
            d = w - t->v.n - ((unsigned)b & D_00299440[e]);
            k -= e;
            b >>= e;
            do {
                d &= 0x1ffff;
                if (d < w)
                    D_0029E468 = (unsigned char *)D_0029E48C;
                else
                    D_0029E468 = (unsigned char *)D_0029E48C - 0x20000;
                n -= (e = (e = 0x20000 - ((d > w) ? d : w)) > n ? n : e);
                if ((unsigned)(w - d) >= e) {
                    memcpy(D_0029E48C + w, D_0029E468 + d, e);
                    w += e;
                    d += e;
                } else {
                    do {
                        D_0029E48C[w++] = D_0029E468[d++];
                    } while (--e);
                }
                if (w == 0x20000) {
                    r = func_00240270(0x20000);
                    if (r)
                        return r;
                    w = 0;
                }
            } while (n != 0);
        }
    }

    D_0029DCCC = w;
    D_0029DCD4 = (unsigned int)b;
    D_0029DCD0 = k;
    D_0029E488 = in;
    return r;
}

extern void func_00254F00(void *);
typedef struct Node { int x0; char *next; } Node;
int func_00240C70(char *ptr) {
    char *old;
    while (ptr) {
        ptr -= 8;
        old = ptr;
        ptr = ((Node *)ptr)->next;
        func_00254F00(old);
    }
    return 0;
}

extern void *func_00254EB0(unsigned int);
int func_00240CB8(unsigned *b, unsigned n, unsigned s, unsigned short *d,
                 unsigned short *e, Huft **t, unsigned *m)
{
    unsigned a;
    unsigned c[17];
    unsigned f;
    unsigned el;
    int g;
    int h;
    register unsigned i;
    register unsigned j;
    register int k;
    int lx[18];
    int *l = lx + 1;
    register unsigned *p;
    register Huft *q;
    Huft r;
    Huft *u[16];
    unsigned v[288];
    register int w;
    unsigned x[17];
    register unsigned *xp;
    int y;
    unsigned z;

    el = 16;
    if (n > 256) el = b[256];
    for (i = 0; i < 17; i++) c[i] = 0;
    p = b; i = n;
    while (i--) c[*p++]++;
    if (c[0] == n) { *t = 0; *m = 0; return 0; }
    for (j = 1; j <= 16; j++) if (c[j]) break;
    k = j;
    if (*m < j) *m = j;
    for (i = 16; i; i--) if (c[i]) break;
    g = i;
    if (*m > i) *m = i;
    for (y = 1 << j; j < i; y <<= 1, j++)
        if ((y -= c[j]) < 0) return 2;
    if ((y -= c[i]) < 0) return 2;
    c[i] += y;
    x[1] = j = 0;
    p = c + 1; xp = x + 2;
    while (--i) *xp++ = (j += *p++);
    for (i = 0; i < 288; i++) v[i] = 0;
    p = b; i = 0;
    for (; i < n; i++) if ((j = *p++) != 0) { v[x[j]] = i; x[j]++; }
    n = x[g];
    x[0] = i = 0;
    p = v;
    h = -1;
    w = l[-1] = 0;
    u[0] = 0;
    z = 0;
    q = 0;
    for (; k <= g; k++) {
        a = c[k];
        while (a--) {
            while (k > w + l[h]) {
                w += l[h++];
                z = g - w;
                if (z > *m) z = *m;
                if ((f = 1 << (j = k - w)) > a + 1) {
                    f -= a + 1;
                    xp = c + k;
                    j++;
                    while (j < z) {
                        if ((f <<= 1) <= *(volatile unsigned *)++xp) break;
                        j++;
                        f -= *xp;
                    }
                }
                if (w + j > el && w < el) j = el - w;
                z = 1 << j;
                l[h] = j;
                q = (Huft *)func_00254EB0((z + 1) * sizeof(Huft));
                if (q == 0) {
                    if (h) func_00240C70((char *)u[0]);
                    return 3;
                }
                *t = q + 1;
                *(t = &q->v.t) = 0;
                u[h] = ++q;
                if (h) {
                    x[h] = i;
                    r.b = l[h - 1];
                    r.e = 16 + j;
                    r.v.t = q;
                    j = (i & ((1 << w) - 1)) >> (w - l[h - 1]);
                    u[h - 1][j] = r;
                }
            }
            r.b = k - w;
            if (p >= v + n) r.e = 99;
            else if (*p < s) {
                r.e = *p < 256 ? 16 : 15;
                r.v.n = *p++;
            } else {
                r.e = e[*p - s];
                r.v.n = d[*p++ - s];
            }
            f = 1 << (k - w);
            for (j = i >> w; j < z; j += f) q[j] = r;
            for (j = 1 << (k - 1); i & j; j >>= 1) i ^= j;
            i ^= j;
            while ((i & ((1 << w) - 1)) != x[h]) w -= l[--h];
        }
    }
    *m = l[0];
    return y != 0 && g != 1;
}


extern unsigned short D_00299446[];
extern unsigned char func_00242058(void);
int func_002413E8(void) {
 unsigned int bits, length; unsigned long buffer; int position, result, ret;
 bits=D_0029DCD0;
 length=bits & D_00299446[0];
 buffer=D_0029DCD4;
 bits-=length;
 buffer>>=length;
 result=0;
 position=D_0029DCCC;
 while(bits<16) { buffer|=(unsigned long)(func_00242058()&255)<<bits; bits+=8; }
 length=(int)buffer & 65535;
 bits-=16; buffer>>=16;
 while(bits<16) { buffer|=(unsigned long)(func_00242058()&255)<<bits; bits+=8; }
 if(length == (int)(~buffer & 65535)) {
 buffer>>=16; bits-=16;
 while(length--) {
 while(bits<8) { buffer|=(unsigned long)(func_00242058()&255)<<bits; bits+=8; }
 D_0029E48C[position++]=buffer;
 if(position==0x20000) { result=func_00240270(0x20000); if(result) goto done; position=0; }
 buffer>>=8; bits-=8;
 }
 D_0029DCCC=position; D_0029DCD4=(int)buffer; D_0029DCD0=bits;
done:
 ret=result;
 } else ret=1;
 return ret;
}

extern int D_0029E478,D_0029E47C,D_0029E480;
extern char D_002994B8[],D_002994F8[],D_00299538[],D_00299578[];

extern int func_002407F8(Huft *,Huft *,int,int);
/* gzip inflate_fixed: build the fixed Huffman tables once, then decode */
int func_00241598(void) {
  if (D_0029DCE0 == 0) {
    int i;
    unsigned l[288];

    for (i = 0; i < 144; i++)
      l[i] = 8;
    for (; i < 256; i++)
      l[i] = 9;
    for (; i < 280; i++)
      l[i] = 7;
    for (; i < 288; i++)
      l[i] = 8;
    D_0029E47C = 7;
    if ((i = func_00240CB8(l, 288, 257, D_002994B8, D_002994F8, &D_0029DCE0, &D_0029E47C)) != 0) {
      D_0029DCE0 = 0;
      return i;
    }
    for (i = 0; i < 30; i++)
      l[i] = 5;
    D_0029E480 = 5;
    if ((i = func_00240CB8(l, 30, 0, D_00299538, D_00299578, (char **)&D_0029E478, &D_0029E480)) > 1) {
      func_00240C70(D_0029DCE0);
      D_0029DCE0 = 0;
      D_0029E478 = 0;
      return i;
    }
  }
  if (D_0029E494) {
    if (D_0029E488 >= D_0029E490 + 0x1b1e0)
      return func_00240358((Huft *)D_0029DCE0, (Huft *)D_0029E478, D_0029E47C, D_0029E480);
  } else if (D_0029E488 > (unsigned char *)0x1ffa9df)
    return func_00240358((Huft *)D_0029DCE0, (Huft *)D_0029E478, D_0029E47C, D_0029E480);
  return func_002407F8(D_0029DCE0, D_0029E478, D_0029E47C, D_0029E480);
}

extern unsigned D_00299468[];
int func_002417E0(void) {
    unsigned l[286+30];
    Huft *tl;
    int bl;
    Huft *td;
    int bd;
    register unsigned long b;
    register unsigned k;
    unsigned m;
    unsigned nl;
    unsigned nd;
    unsigned nb;
    unsigned n;
    int i;
    unsigned j;
    unsigned last;
    Huft *t;

    b = D_0029DCD4;
    k = D_0029DCD0;
    while (k < 5) {
        b |= (unsigned long)(func_00242058() & 255) << k;
        k += 8;
    }
    nl = 257 + (b & D_00299440[5]);
    b >>= 5;
    k -= 5;
    while (k < 5) {
        b |= (unsigned long)(func_00242058() & 255) << k;
        k += 8;
    }
    nd = 1 + (b & D_00299440[5]);
    b >>= 5;
    k -= 5;
    while (k < 4) {
        b |= (unsigned long)(func_00242058() & 255) << k;
        k += 8;
    }
    nb = 4 + (b & D_00299440[4]);
    b >>= 4;
    k -= 4;
    if (nl > 286 || nd > 30)
        return 1;
    for (j = 0; j < nb; j++) {
        while (k < 3) {
            b |= (unsigned long)(func_00242058() & 255) << k;
            k += 8;
        }
        l[D_00299468[j]] = b & 7;
        b >>= 3;
        k -= 3;
    }
    for (; j < 19; j++)
        l[D_00299468[j]] = 0;
    bl = 7;
    i = func_00240CB8(l, 19, 19, 0, 0, (char **)&tl, &bl);
    if (bl == 0)
        i = 1;
    if (i != 0) {
        if (i == 1)
            func_00240C70((char *)tl);
    }
    n = nl + nd;
    m = D_00299440[bl];
    last = 0;
    i = 0;
    if (n != 0) {
        do {
        while (k < bl) {
            b |= (unsigned long)(func_00242058() & 255) << k;
            k += 8;
        }
        td = tl + ((unsigned)b & m);
        j = td->b;
        b >>= j;
        k -= j;
        j = td->v.n;
        if (j < 16) {
            l[i++] = last = j;
        } else if (j == 16) {
            while (k < 2) {
                b |= (unsigned long)(func_00242058() & 255) << k;
                k += 8;
            }
            j = 3 + (b & D_00299440[2]);
            b >>= 2;
            k -= 2;
            if (i + j > n)
                return 1;
            while (j--)
                l[i++] = last;
        } else if (j == 17) {
            while (k < 3) {
                b |= (unsigned long)(func_00242058() & 255) << k;
                k += 8;
            }
            j = 3 + (b & D_00299440[3]);
            b >>= 3;
            k -= 3;
            if (i + j > n)
                return 1;
            while (j--)
                l[i++] = 0;
            last = 0;
        } else {
            while (k < 7) {
                b |= (unsigned long)(func_00242058() & 255) << k;
                k += 8;
            }
            j = 11 + (b & D_00299440[7]);
            b >>= 7;
            k -= 7;
            if (i + j > n)
                return 1;
            while (j--)
                l[i++] = 0;
            last = 0;
        }
        } while (i < n);
    }
    func_00240C70((char *)tl);
    D_0029DCD4 = (unsigned int)b;
    D_0029DCD0 = k;
    bl = 9;
    i = func_00240CB8(l, nl, 257, D_002994B8, D_002994F8, (char **)&tl, &bl);
    if (bl == 0)
        i = 1;
    if (i != 0) {
        if (i == 1)
            func_00240C70((char *)tl);
        return i;
    }
    bd = 6;
    i = func_00240CB8(l + nl, nd, 0, D_00299538, D_00299578, (char **)&td, &bd);
    if (bd == 0 && nl > 257)
        i = 1;
    if (i != 0) {
        if (i == 1)
            func_00240C70((char *)td);
        func_00240C70((char *)tl);
        return i;
    }
    if (D_0029E494) {
        if (D_0029E488 < D_0029E490 + 0x1b1e0)
            i = func_002407F8(tl, td, bl, bd);
        else
            i = func_00240358(tl, td, bl, bd);
    } else {
        if (D_0029E488 <= (unsigned char *)0x1ffa9df)
            i = func_002407F8(tl, td, bl, bd);
        else
            i = func_00240358(tl, td, bl, bd);
    }
    func_00240C70((char *)tl);
    func_00240C70((char *)td);
    return i;
}

extern const unsigned short D_00299440[]; /* mask_bits */



extern int func_00241598(void);
extern int func_002417E0(void);

void func_00241E70(int *e)
{
    unsigned int t;
    unsigned long b;
    unsigned int k;

    b = D_0029DCD4;
    k = D_0029DCD0;

    while (k < 1) {
        b |= (unsigned long)(unsigned char)func_00242058() << k;
        k += 8;
    }
    *e = (int)b & D_00299440[1];
    b >>= 1;
    k -= 1;

    while (k < 2) {
        b |= (unsigned long)(unsigned char)func_00242058() << k;
        k += 8;
    }
    t = (unsigned int)b & D_00299440[2];
    b >>= 2;
    k -= 2;

    D_0029DCD4 = b;
    D_0029DCD0 = k;

    if (t == 0) {
        func_002413E8();
    } else if (t == 1) {
        func_00241598();
    } else if (t == 2) {
        func_002417E0();
    }
}

extern int func_0020DB10(int a, void *dst, int sector);
extern int D_0029E484;
int func_00241F90(int req, unsigned char *dst)
{
    int done = 0;
    int result;
    int status;

    D_0029DCE4 = req;
    D_0029DCE8[0] = (int)dst;
    D_0029DCD4 = 0;
    D_0029DCD0 = 0;
    D_0029DCCC = 0;
    D_0029E494 = 0;
    D_0029E48C = (char *)dst;
    D_0029E468 = dst;
    D_0029DCC8 = 1;

    if (func_0020DB10(req, (void *)0x1FDF800, 0) == 0)
        return 0;
    result = func_002402E0();
    if (result == 0)
        return 0;
    D_0029E484 = 0;
    do {
        D_0029E484++;
        status = ((int (*)(int *))func_00241E70)(&done);
        if (status != 0)
            return status;
    } while (done == 0);
    func_00240270((unsigned int)D_0029DCCC);
    return result;
}

extern unsigned char *func_00231798(void);
extern int func_0020DB10(int a, void *dst, int sector);

/* Returns the next byte of the stream, refilling the buffer when the cursor runs off its end. */
unsigned char func_00242058(void)
{
    if (D_0029E494 != 0) {
        if (D_0029E490 + 0x1FFFF < D_0029E488) {
            D_0029E490 = D_0029E488 = func_00231798();
        }
        return *D_0029E488++;
    }
    if ((unsigned char *)0x1FFF7FF < D_0029E488) {
        func_0020DB10(D_0029DCE4, (void *)0x1FDF800, D_0029DCC8++);
        D_0029E488 = (unsigned char *)0x1FDF800;
    }
    return *D_0029E488++;
}

extern int D_0029E484;
extern int func_002421C0(unsigned char *src);

int func_00242118(unsigned char *dst, unsigned char *src)
{
    int done = 0;
    int result;
    int status;

    D_0029E494 = 1;
    D_0029DCE8[0] = (int)dst;
    D_0029DCD4 = 0;
    D_0029DCD0 = 0;
    D_0029DCCC = 0;
    D_0029DCC8 = 0;
    D_0029E48C = (char *)dst;
    D_0029E468 = dst;

    result = func_002421C0(src);
    if (result == 0)
        return 0;
    D_0029E484 = 0;
    do {
        D_0029E484++;
        status = ((int (*)(int *))func_00241E70)(&done);
        if (status != 0)
            return status;
    } while (done == 0);
    func_00240270((unsigned int)D_0029DCCC);
    return result;
}
